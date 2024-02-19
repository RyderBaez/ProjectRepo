package RyderAssignment2;

import java.util.Vector;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class StockBroker extends Thread {
	private long mTimeOffset;
	private Vector<Schedule> mTrades;
	private static int activeThreads = 0;
	private static int balance = -1;

	private static Lock DeathLock= new ReentrantLock();
	private static Lock brokerLock= new ReentrantLock();
	private static Lock vectorLock= new ReentrantLock();
	StockBroker(int bal, Vector<Schedule> trades, long offset){
		mTrades = trades;
		if(balance == -1) {
		balance = bal;
		}
		DeathLock.lock();
		activeThreads++;
		DeathLock.unlock();
		mTimeOffset = offset;
	}


public void run() {
	long curTime;
	while(!mTrades.isEmpty()) {
		curTime = System.currentTimeMillis();
		long tradeTime = curTime - mTimeOffset;
		if(mTrades.isEmpty()) {
			DeathLock.lock();
			activeThreads--;
			if(activeThreads == 0) {
				System.out.println("All trades completed!");
			}
			DeathLock.unlock();
			return;
		}
		//System.out.println(tradeTime);
		boolean canAct = false;
		Schedule task = new Schedule();
		int newbal;
		vectorLock.lock();
		if(!mTrades.isEmpty()) {
		if(mTrades.get(0).getStartTime() <= (tradeTime/1000)) {
			task = mTrades.remove(0);
			canAct = true;
		}
		}
		vectorLock.unlock();
		if(canAct)
		{
			newbal = UpdateBalance(task.getAmount() * task.getPrice());
			
			if(task.getAmount() < 0) { //we selling
				long hours = (tradeTime / 1000) / 360;
				long minutes = (tradeTime / 1000) / 60;
				long seconds = tradeTime / 1000;
				long miliseconds = tradeTime % 1000;
				if(miliseconds < 10) {
					System.out.println("[" + hours + ":" + minutes + ":" + seconds + ".00" + 
					miliseconds + "] " + "Starting sale of " + (-1*task.getAmount())
					+" stocks of " + task.getTicker());
				}
				else if (miliseconds < 100) {
					System.out.println("[" + hours + ":" + minutes + ":" + seconds + ".0" + 
					miliseconds + "] " + "Starting sale of " + (-1*task.getAmount())
					+" stocks of " + task.getTicker());
				}
				else {
				System.out.println("[" + hours + ":" + minutes + ":" + seconds + "." + 
				miliseconds + "] " + "Starting sale of " + (-1*task.getAmount())
				+" stocks of " + task.getTicker());
				}
				try {
				Thread.sleep(((long)3000) + curTime - System.currentTimeMillis());
				} catch (InterruptedException ie) {
					System.out.println("Trade Interupted");
				}
				if(newbal == -1) {
					System.out.println("Transaction failed due to insufficient balance. Unsuccessful purchase\r\n"
							+ "of "+ (-1*task.getAmount())+" stocks of " + task.getTicker());
				}
				else {
					tradeTime = System.currentTimeMillis() - mTimeOffset;
					hours = (tradeTime / 1000) / 360;
					minutes = (tradeTime / 1000) / 60;
					seconds = tradeTime / 1000;
					miliseconds = tradeTime % 1000;
					if(miliseconds < 10) {
						System.out.println("[" + hours + ":" + minutes + ":" + seconds + ".00" + 
						miliseconds + "] " + "Finished sale of " + (-1*task.getAmount())
						+" stocks of " + task.getTicker() + "\nCurrent Balance after trade: " + newbal);
					}
					else if (miliseconds < 100) {
						System.out.println("[" + hours + ":" + minutes + ":" + seconds + ".0" + 
						miliseconds + "] " + "Finished sale of " + (-1*task.getAmount())
						+" stocks of " + task.getTicker() + "\nCurrent Balance after trade: " + newbal);
					}
					else {
					System.out.println("[" + hours + ":" + minutes + ":" + seconds + "." + 
					miliseconds + "] " + "Finished sale of " + (-1*task.getAmount())
					+" stocks of " + task.getTicker() + "\nCurrent Balance after trade: " + newbal);
					}
				}
			}
			
			else {//we buying
				long hours = (tradeTime / 1000) / 360;
				long minutes = (tradeTime / 1000) / 60;
				long seconds = tradeTime / 1000;
				long miliseconds = tradeTime % 1000;
				if(miliseconds < 10) {
					System.out.println("[" + hours + ":" + minutes + ":" + seconds + ".00" + 
					miliseconds + "] " + "Starting purchase of " + (task.getAmount())
					+" stocks of " + task.getTicker());
				}
				else if (miliseconds < 100) {
					System.out.println("[" + hours + ":" + minutes + ":" + seconds + ".0" + 
					miliseconds + "] " + "Starting purchase of " + (task.getAmount())
					+" stocks of " + task.getTicker());
				}
				else {
				System.out.println("[" + hours + ":" + minutes + ":" + seconds + "." + 
				miliseconds + "] " + "Starting purchase of " + (task.getAmount())
				+" stocks of " + task.getTicker());
				}
				try {
					Thread.sleep(((long)2000) + curTime - System.currentTimeMillis());
					} catch (InterruptedException ie) {
						System.out.println("Trade Interupted");
					}
				if(newbal == -1) {
					System.out.println("Transaction failed due to insufficient balance. Unsuccessful purchase\r\n"
							+ "of "+ (task.getAmount())+" stocks of " + task.getTicker());
				}
				else {
					tradeTime = System.currentTimeMillis() - mTimeOffset;
					hours = (tradeTime / 1000) / 360;
					minutes = (tradeTime / 1000) / 60;
					seconds = tradeTime / 1000;
					miliseconds = tradeTime % 1000;
					if(miliseconds < 10) {
						System.out.println("[" + hours + ":" + minutes + ":" + seconds + ".00" + 
						miliseconds + "] " + "Finished purchase of " + (task.getAmount())
						+" stocks of " + task.getTicker() + "\nCurrent Balance after trade: " + newbal);
					}
					else if (miliseconds < 100) {
						System.out.println("[" + hours + ":" + minutes + ":" + seconds + ".0" + 
						miliseconds + "] " + "Finished purchase of " + (task.getAmount())
						+" stocks of " + task.getTicker() + "\nCurrent Balance after trade: " + newbal);
					}
					else {
					System.out.println("[" + hours + ":" + minutes + ":" + seconds + "." + 
					miliseconds + "] " + "Finished purchase of " + (task.getAmount())
					+" stocks of " + task.getTicker() + "\nCurrent Balance after trade: " + newbal);
					}
				}
			}
			
		}
	}
	DeathLock.lock();
	activeThreads--;
	if(activeThreads == 0) {
		System.out.println("All trades completed!");
	}
	DeathLock.unlock();
	return;
}
private static int UpdateBalance(int cost) {
	brokerLock.lock();
	if(balance - cost < 0) {
		brokerLock.unlock();
		return -1;
	}
	balance -= cost;
	brokerLock.unlock();
	return balance;
}
}
