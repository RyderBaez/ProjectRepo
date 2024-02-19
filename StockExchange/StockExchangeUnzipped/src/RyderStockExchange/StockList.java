package RyderAssignment2;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.Vector;
import java.util.concurrent.ExecutorService;

//generated get/set from https://www.jsonschema2pojo.org/ given by Prof
import com.google.gson.*;
import java.io.FileWriter;
import java.io.IOException;
public class StockList {
//https://www.jsonschema2pojo.org/ generated starting values given by prof
	private List<Stock> data;

	public List<Stock> getData() {
	return data;
	}

	public void setData(List<Stock> data) {
	this.data = data;
	}
	
	public Stock searchByTicker(String Ticker) { //searches for if a ticker is present in the 
		for(int i = 0; i < data.size(); i ++) {
			if(Ticker.compareToIgnoreCase(data.get(i).getTicker()) == 0) {
				//printStock((data.get(i)), false);
				return data.get(i);
			}
		}
		//System.out.println(Ticker + " could not be found.");
		return null;
	}
	
	public void start(int bal, Vector<Schedule> tasks) {
		for(int i = 0; i < tasks.size(); i++) { //gives each task to its associated Stock
			(searchByTicker(tasks.elementAt(i).getTicker())).AddTask(tasks.elementAt(i));
		}
		ArrayList<ArrayList<ExecutorService>> stockThreads = new ArrayList<ArrayList<ExecutorService>>();
		for(int i = 0; i < data.size(); i++) {
			//System.out.println(i);
			stockThreads = data.get(i).Initialize(stockThreads);
		}
		long time = System.currentTimeMillis();
		for(int i = 0; i < stockThreads.size(); i++) {
			for(int j = 0; j < stockThreads.get(i).size(); j++) {
			stockThreads.get(i).get(j).execute(new StockBroker(bal, data.get(i).getTasks(), time));
			}
			}
		
			for(int i = 0; i < stockThreads.size(); i++) {
				for(int j = 0; j < stockThreads.get(i).size(); j++) {
				//while(!(((stockThreads.get(i)).get(j)).isTerminated())) {
					
				//}
				//System.out.println("hey");
				stockThreads.get(i).get(j).shutdown();
				}
				
			}
		
	}
	
	
	
	
	public void printStock(Stock S, boolean withDescription) {
		String temp = "";
		temp += S.getName();//does proper form for printing
		temp += ", symbol ";
		temp += S.getTicker();
		temp += ", started on ";
		temp += S.getStartDate();
		temp += ", listed on ";
		temp += S.getExchangeCode();
		temp += ",";
		
		int charcnt = 0;
		for(int i = 0; i < temp.length(); i++) { //formats description
			charcnt++;
			if(temp.charAt(i) == ' ' && charcnt > 60) {//splits into unique lines every next space after 60 chars
				charcnt = 0;
				temp = temp.substring(0, i) + '\n' + temp.substring(i+1);
			}
			
		}
		System.out.println(temp);//print them!
		temp = "\t";
		
		if(withDescription) {
			temp += S.getDescription();
			
		charcnt = 0;
		for(int i = 0; i < temp.length(); i++) { //formats description
			charcnt++;
			if(temp.charAt(i) == ' ' && charcnt > 60) {//splits into unique lines every next space after 60 chars
				charcnt = 0;
				temp = temp.substring(0, i) + '\n' + '\t' + temp.substring(i+1);
			}
			
		}
		System.out.println(temp);//print them!
		}
	}
	
	
	public void printAllStocks() {
		for (int i = 0; i < data.size(); i++) {
			printStock(data.get(i), true);//prints every stock
		}
	}
	
	public void addNewStock(Scanner scan) {
		boolean isNotValid = true;
		Stock newStock = new Stock(); //creates new stock
		while(isNotValid) {
			System.out.println("What is the name of the company you would like to add?");
			String name = scan.nextLine();
			if(name.equals("")) { //prompts options and checks if they are valid
				continue;
			} 
			boolean repeat = false;
			for(int i = 0; i < data.size(); i++) {
				if(data.get(i).getName().equalsIgnoreCase(name)) {
					repeat = true;
				}
			}
			if(repeat || name == null) {
				if(name!= null) {
				System.out.println("There is already an entree for " + name);
				}
				continue;
			}
			newStock.setName(name);
			System.out.println("What is the stock symbol of " + name + "?");
			String Ticker = scan.nextLine();
			
			for(int i = 0; i < data.size(); i++) {
				if(data.get(i).getTicker().equalsIgnoreCase(Ticker)) {
					repeat = true;
				}
			}
			if(repeat || Ticker == null || Ticker.equals("")) {
				if(repeat) {
				System.out.println(Ticker + "is already in the stock market");
				}
				continue;
			}
			newStock.setTicker(Ticker);
			System.out.println("What is the start date of " + name + "?");
			String Date = scan.nextLine();
			if(Date == null) {
				System.out.println("Not a Valid Date");
				continue;
			}
			if(Date.length() != 10) {
			System.out.println("Not a Valid Date");
			continue;
			}
			else {
				if(Date.charAt(4) != '-') {
					System.out.println("Not a Valid Date");
					continue;
				}
				if(Date.charAt(7) != '-') {
					System.out.println("Not a Valid Date");
					continue;
				}
				boolean Invalid = false;
				for(int i = 0; i < 4; i++) {
					if(Date.charAt(i) < '0' || Date.charAt(i) > '9') {
						Invalid = true;
					}
				}
				for(int i = 5; i < 7; i++) {
					if(Date.charAt(i) < '0' || Date.charAt(i) > '9') {
						Invalid = true;
					}
				}
				for(int i = 8; i < Date.length(); i++) {
					if(Date.charAt(i) < '0' || Date.charAt(i) > '9') {
						Invalid = true;
					}
				}
				if(Invalid) {
					System.out.println("Not a Valid Date");
					continue;
				}
				newStock.setStartDate(Date);
			}
			System.out.println("What is the exchange where " + name + " is listed?");
			String Exchange = scan.nextLine();
			if(Exchange.equalsIgnoreCase("NASDAQ")) {
				newStock.setExchangeCode("NASDAQ");
			}
			else if (Exchange.equalsIgnoreCase("NYSE")) {
				newStock.setExchangeCode("NYSE");
			}
			else {
				System.out.println("Invalid Exchange Code");
				continue;
			}
			System.out.println("What is the description of " + name + "?");
			String Description = scan.nextLine();
			if(Description.equalsIgnoreCase("") || Description == null) {
				continue;
			}
			newStock.setDescription(Description);
			isNotValid = false;
			}
		data.add(newStock);//finally adds
		System.out.println("There is now a new entry for: ");
		printStock(newStock, true);
	}
	public void findAllExchange(String Exchange) { //checks everything on the given exchange
		List<String> onExchange = new ArrayList<String>();
		for (int i = 0; i < data.size(); i++) {
			if(((data.get(i)).getExchangeCode()).equalsIgnoreCase(Exchange)) {
			onExchange.add(data.get(i).getTicker());
			}
		}
		if(onExchange.size() <= 0) {
			if(Exchange.equalsIgnoreCase("NYSE") || Exchange.equalsIgnoreCase("NASDAQ")) {
				System.out.println("No stocks on " + Exchange);
				return;
			}
			System.out.println("No exchange named " + Exchange + " found.");
			return;
		}
		String temp = onExchange.get(0);
		
		for(int i = 1; i < onExchange.size() - 1; i++) {
			temp += ", ";
			temp += onExchange.get(i);
		}
		temp += ", and ";
		temp += onExchange.get(onExchange.size() - 1);
		temp += " found on the ";
		temp += Exchange;
		temp += " exchange."; //proper formating
		int charcnt = 0;
		for(int i = 0; i < temp.length(); i++) {
			charcnt++;
			if(temp.charAt(i) == ' ' && charcnt > 60) { //correcting for too long a string
				charcnt = 0;
				temp = temp.substring(0, i) + '\n' + temp.substring(i+1);
			}
			
		}
		System.out.println(temp);
		
	}
	
	
	public void SaveData(String FileName) {
		Gson gsonBuild = new GsonBuilder().setPrettyPrinting().create(); //https://howtodoinjava.com/gson/pretty-print-json-output/
		//System.out.println(gsonBuild.toJson(data, StockList.class));
		FileWriter myWriter = null;
		try {
		  myWriter = new FileWriter(FileName); //https://www.w3schools.com/java/java_files_create.asp
	      gsonBuild.toJson(this, StockList.class, myWriter);//puts it into the file
	      if(myWriter != null) {
		      myWriter.close();
			}
		}
		catch(IOException e){
			System.out.println("Failed to Write to File");
		}
	}
	
	
	
	
	
	public void removeStock(Scanner scan) {//removes a stock
		int Index;
		boolean notValid = true;
		if(data.size() == 0) {
			System.out.println("No Companies to Remove");
			return;
		}
		while(notValid) {
		for(int i = 0; i < data.size(); i++) {//shows all stocks to remove
			System.out.println("\t" + Integer.toString(i + 1) + ") " + data.get(i).getName());
			
		}
			System.out.println("Which company would you like to remove?");
			String Input = scan.nextLine();
			try {
				Index = Integer.parseInt(Input);
			}
			catch(NumberFormatException e) {
				System.out.println("Invalid Index");
				continue;
			}
			if((Index - 1) < data.size()) {//if a valid index remove!
				System.out.println(data.get(Index-1).getName() + " has been removed.");
				data.remove(Index - 1);
				notValid = false;
			}
			else {
				System.out.println("Invalid Index");
			}
		}
		}

	
	
	public void sortData(boolean order) { //true does largest to smallest false does smallest to largest
		if(!order) {
			for(int i = 1; i < data.size(); i++) {
				for(int j = 0; j < data.size() - i; j++) {
					int dif = data.get(j).getName().compareToIgnoreCase(data.get(j + 1).getName());
					if(dif < 0) {
						Stock temp = data.get(j);
						data.set(j, data.get(j+1));
						data.set(j+1, temp);
					}
				}
			}
		}
		else {
			for(int i = 1; i < data.size(); i++) {
				for(int j = 0; j < data.size() - i; j++) {
					int dif = data.get(j).getName().compareToIgnoreCase(data.get(j + 1).getName());
					if(dif > 0) {
						Stock temp = data.get(j);
						data.set(j, data.get(j+1));
						data.set(j+1, temp);
					}
				}
			}
		}
	}
	}

