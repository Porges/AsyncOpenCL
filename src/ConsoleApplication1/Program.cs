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
            using (var buffer = ctx.CreateBuffer((ulong)maxBytes))
            using (var queue = new CommandQueue(ctx, device))
            {
                for (int bytes = 1; bytes <= maxBytes; ++bytes)
                {
                    await RunCopies(bytes, queue, buffer);
                }
            }
        }

        private static async Task RunCopies(int bytes, CommandQueue queue, Buffer buffer)
        {
            var src = new byte[bytes];
            var dst = new byte[bytes];

            for (int i = 0; i < src.Length; ++i)
                src[i] = (byte) i;

            using (var writeEvent = queue.EnqueueWrite(buffer, src))
            using (var readEvent = queue.EnqueueRead(buffer, dst, after: writeEvent))
            {
                queue.Flush();
                await readEvent;

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
