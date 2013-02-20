using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Contexts;
using System.Threading.Tasks;
using OpenCL;
using Buffer = OpenCL.Buffer;
using Context = OpenCL.Context;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                Go().Wait();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex);
            }
        }

        static async Task Go()
        {
            int maxBytes = 10000;
            var device = Platform.GetPlatforms().First().GetDevices(DeviceType.Gpu).First();
            using (var ctx = new Context(device))
            using (var queue = new CommandQueue(ctx, device))
            {
                var tasks = new List<Task>();
                for (int bytes = 1; bytes <= maxBytes; ++bytes)
                    tasks.Add(RunCopies(bytes, queue, ctx));

                // nothing runs until...
                queue.Flush(); // pow!

                // now wait for them all to finish
                await Task.WhenAll(tasks);
            }
        }

        private static async Task RunCopies(int bytes, CommandQueue queue, Context ctx)
        {
            using (var buffer = new Buffer(ctx, (ulong) bytes))
            {
                var src = new byte[bytes];
                var dst = new byte[bytes];

                for (int i = 0; i < src.Length; ++i)
                    src[i] = (byte) i;

                // queue a write to the buffer
                using (var writeEvent = queue.EnqueueWrite(buffer, src))

                // queue a read to happen after the write completes
                using (var readEvent = queue.EnqueueRead(buffer, dst, after: writeEvent))
                {
                    // wait for the read to complete before continuing
                    await readEvent;

                    // if we don't await then `dst` might not be populated
                    if (!src.SequenceEqual(dst))
                    {
                        Console.WriteLine("Error! Not equal:");
                        Console.WriteLine(string.Join(",", src));
                        Console.WriteLine(string.Join(",", dst));
                    }
                }
            }
        }
    }
}
