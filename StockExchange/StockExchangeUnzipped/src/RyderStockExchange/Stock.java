//generated get/set from https://www.jsonschema2pojo.org/ given by Prof
package RyderAssignment2;

import java.util.ArrayList;
import java.util.Vector;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Stock {
	
	private String name;

	private String ticker;

	private String startDate;

	private int stockBrokers;

	private String description;

	private String exchangeCode;

	public String getName() {
	return name;
	}

	public void setName(String name) {
	this.name = name;
	}

	public String getTicker() {
	return ticker;
	}

	public void setTicker(String ticker) {
	this.ticker = ticker;
	}

	public String getStartDate() {
	return startDate;
	}

	public void setStartDate(String startDate) {
	this.startDate = startDate;
	}
	
	public int getStockBrokers() {
		return stockBrokers;
	}

	public void setStockBrokers(int stockBrokers) {
		this.stockBrokers = stockBrokers;
	}

	public String getDescription() {
	return description;
	}

	public void setDescription(String description) {
	this.description = description;
	}

	public String getExchangeCode() {
	return exchangeCode;
	}

	public void setExchangeCode(String exchangeCode) {
	this.exchangeCode = exchangeCode;
	}
	
	private Vector<Schedule> mTasks = new Vector<Schedule>();
	
	public void AddTask(Schedule task) {
		mTasks.add(task);
	}
	public Vector<Schedule> getTasks(){
		return mTasks;
	}
	public ArrayList<ArrayList<ExecutorService>> Initialize(ArrayList<ArrayList<ExecutorService>> stockThreads) {
		ArrayList<ExecutorService> stockBrokerThreads = new ArrayList<ExecutorService>();
		for(int i = 0; i < stockBrokers; i++) {
			stockBrokerThreads.add(Executors.newFixedThreadPool(1));
		}
		stockThreads.add(stockBrokerThreads);
		return stockThreads;
	}
	}

