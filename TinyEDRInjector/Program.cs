using System.Diagnostics;
using System.Management;
using System.Security.Principal;

namespace TinyEDRInjector;

class Program {

    private static string process;
    private static string dllPath;

    static void Main(string[] args)
    {

        RunThisAsAdmin(string.Join(" ", args));

        process = args[0];
        dllPath = args[1];
        
        Console.WriteLine($"{args[0]}, {args[1]}");

        try
        {
            new Thread(WaitForProcess) { IsBackground = true, Name = "worker" }.Start();
            Console.WriteLine("Waiting for process events");
            do
            {
                Thread.Sleep(5000);
            } while (true);

        }
        catch (Exception e)
        {
            Console.WriteLine($"Failed to setup injector ${e.Message}");
        }
    }

    static void WaitForProcess()
    {
        try
        {
            var startWatch = new ManagementEventWatcher(new WqlEventQuery("SELECT * FROM Win32_ProcessStartTrace"));
            startWatch.EventArrived += new EventArrivedEventHandler(startWatch_EventArrived);
            startWatch.Start();

            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine($"+ Listening for the following processes: {string.Join(" ", process)}\n");
        }
        catch (Exception ex)
        {
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine(ex);
        }
    }

    static void startWatch_EventArrived(object sender, EventArrivedEventArgs e)
    {
        try
        {
            var proc = GetProcessInfo(e);
            if (process.Contains(proc.ProcessName.ToLower()))
            {
                Console.ForegroundColor = ConsoleColor.Green;
                Console.WriteLine($" Injecting process {proc.ProcessName}({proc.PID}) with DLL {dllPath}");
                BasicInject.Inject(proc.PID, dllPath);
            }
        }
        catch (Exception ex)
        {
            Console.ForegroundColor = ConsoleColor.Yellow;
            Console.WriteLine(ex);
        }
    }

    static ProcessInfo GetProcessInfo(EventArrivedEventArgs e)
    {
        var p = new ProcessInfo();
        var pid = 0;
        int.TryParse(e.NewEvent.Properties["ProcessID"].Value.ToString(), out pid);
        p.PID = pid;
        p.ProcessName = e.NewEvent.Properties["ProcessName"].Value.ToString();
        return p;
    }

    internal class ProcessInfo
    {
        public string ProcessName { get; set; }
        public int PID { get; set; }
    }

    private static void RunThisAsAdmin(string args)
    {
        if (!IsAdministrator())
        {
            var exe = Process.GetCurrentProcess().MainModule.FileName;
            var startInfo = new ProcessStartInfo(exe, args)
            {
                UseShellExecute = true,
                Verb = "runas",
                WindowStyle = ProcessWindowStyle.Normal,
                CreateNoWindow = false,

            };
            Process.Start(startInfo);
            Process.GetCurrentProcess().Kill();
        }
    }

    private static bool IsAdministrator()
    {
        var identity = WindowsIdentity.GetCurrent();
        var principal = new WindowsPrincipal(identity);
        return principal.IsInRole(WindowsBuiltInRole.Administrator);
    }
}