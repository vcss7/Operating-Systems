/* those trying to test this programs on your high end laptops, 
might see no matter you use a counter of 1000 or 5000 or even 10,000 to increment,
you might still get the correct total of incremented value without even 
using SYNCHRONIZE keyword to sync the increment process between 2 threads. 
It just  means your thread scheduler has a lot of resources available to be very efficient.
 In such cases, just increase the counter loop by a million and then you will see
 the need of using the SYNCHRONIZE keyword as you will see an incorrect total. */

class Counter
	{
		int count;
		public synchronized void increment()
		{
			count++;
		}
	}

public class SynchDemo
{
	public static void main(String[] args) throws Exception
	{
		Counter c = new Counter();

		Thread t1 = new Thread(new Runnable()
		{
			public void run()
			{
				for (int i = 1; i <= 100000; i++)
				{
					c.increment();
				}
			}
		});

		Thread t2 = new Thread(new Runnable()
		{
			public void run()
			{
				for (int i = 1; i <= 100000; i++)
				{
					c.increment();
				}
			}
		});

		t1.start();
		t2.start();
		t1.join();
		t2.join();
		System.out.println("Count " + c.count);
	}	
}		

