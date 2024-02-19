package RyderAssignment2;
import com.google.gson.*;

import java.util.List;
import java.util.Scanner;
import java.util.Vector;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import com.opencsv.*;
import com.opencsv.exceptions.CsvException;

public class StockSearching {

	public static void main(String[] args) {
		String FileName = null;
		StockList Stocks = new StockList();
		Vector<Schedule> allActions = new Vector<Schedule>();
		Scanner scan = new Scanner(System.in);
		//GsonBuilder builder = new GsonBuilder(); 
		//builder.setPrettyPrinting(); 
		Gson gson = new Gson();
		boolean InValidFile = true;
		while(InValidFile) {
			System.out.println("Give a Json File Name");
			FileName = scan.nextLine();
			File myObj = new File(FileName);
			String temp = "";
			Scanner inFile = null;
			try {
				inFile = new Scanner(myObj);
				
				while(inFile.hasNext()) {
					temp += inFile.nextLine();
				}
			}
			catch(FileNotFoundException e) {
				System.out.println("The file " + FileName + " could not be found.");
				continue;
			}
			finally {
				if(inFile != null) {
				inFile.close();
				}
			}
			try {
				Stocks = gson.fromJson(temp, StockList.class);
				}
			catch(JsonSyntaxException e) {
				System.out.println("The File " + FileName + " is not formatted properly.");
				continue;
			}
			if(Stocks == null) {
				System.out.println("File " + FileName +  " is Empty");
				continue;
			}
			List<Stock> Data = Stocks.getData();
			if(Data.size() == 0) {
				System.out.println("File " + FileName +  " is Empty");
				continue;
			}
			boolean Invalid = false;
			for(int i = 0; i < Data.size(); i++) {
				if(Data.get(i).getName().equals("") || Data.get(i).getName() == null) {
					Invalid = true;
				}
				if(Data.get(i).getTicker().equals("") || Data.get(i).getTicker() == null) {
					Invalid = true;
				}
				if(Data.get(i).getStockBrokers() <= 0) {
					Invalid = true;
				}
				if(Data.get(i).getDescription().equals("") || Data.get(i).getDescription() == null) {
					Invalid = true;
				}
				if(Data.get(i).getStartDate().equals("") || Data.get(i).getStartDate() == null) {
					Invalid = true;
				}
				if(Data.get(i).getExchangeCode().equals("") || Data.get(i).getExchangeCode() == null) {
					Invalid = true;
				}
				String Date = Data.get(i).getStartDate();
				if(Date.length() != 10) {
					Invalid = true;
					}
					else {
						if(Date.charAt(4) != '-') {
							Invalid = true;
						}
						if(Date.charAt(7) != '-') {
							Invalid = true;
						}
						for(int j = 0; j < 4; j++) {
							if(Date.charAt(j) < '0' || Date.charAt(j) > '9') {
								Invalid = true;
							}
						}
						for(int j = 5; j < 7; j++) {
							if(Date.charAt(j) < '0' || Date.charAt(j) > '9') {
								Invalid = true;
							}
						}
						for(int k = 8; k < Date.length(); k++) {
							if(Date.charAt(k) < '0' || Date.charAt(k) > '9') {
								Invalid = true;
							}
						}
						
						String Exchange = Data.get(i).getExchangeCode();
						if(Exchange.equalsIgnoreCase("NASDAQ")) {
						}
						else if (Exchange.equalsIgnoreCase("NYSE")) {
						}
						else {
							Invalid = true;
						}
					}
			}
			if(Invalid) {
				System.out.println("File " + FileName + " has a Bad Value");
				Data.clear();
				continue;
			}
			InValidFile = false;
		}
		System.out.println("The file " + FileName + " has been properly read!");
		InValidFile = true;
		List<String[]> allLines;
		FileReader filereader = null;
		CSVReader csvReader = null;
		String fileName = "";
		while(InValidFile) {
			System.out.println("Give a CSV File Name");
			fileName = scan.nextLine();
		try { 
			filereader = (new FileReader(fileName));
		        try {
		        	csvReader = new CSVReaderBuilder((new FileReader(fileName))).build(); 
		        	allLines = csvReader.readAll();
		        }
		        catch(IOException e) {
		        	System.out.println("File " + FileName +  " is Empty");
					continue;
		        }
		        catch(CsvException e) {
		        	System.out.println("The File " + FileName + " is not formatted properly.");
					continue;
		        }
		        finally {
		        	if(csvReader != null) {
		        		try {
							csvReader.close();
						} catch (IOException e) {
							System.out.println("could not close csvReader");
						}
		        	}
		        }
		    }
		catch(FileNotFoundException e) {
			System.out.println("The file " + fileName + " could not be found.");
			continue;
		}
			finally {
				if(filereader != null) {
	        		try {
						filereader.close();
					} catch (IOException e) {
						System.out.println("could not close File");
					}
	        	}
			}
		boolean badRead = false;
		for(int i = 0; i < allLines.size(); i++) {
				if(allLines.get(i).length != 4) {
					badRead = true;
					continue;
				}
				Schedule action = new Schedule();
				int time = -1;
				try {
					time = Integer.parseInt(allLines.get(i)[0]);
				}
				catch(NumberFormatException e) {
					badRead = true;
				}
				finally {
				}
				if(time < 0) {
					badRead = true;
				}
				boolean ValidTicker = false;
				for(int j = 0; j < Stocks.getData().size(); j++) {
					if(Stocks.getData().get(j).getTicker().equalsIgnoreCase(allLines.get(i)[1])) {
						ValidTicker = true;
					}
				}
				if(!ValidTicker) {
					badRead = true;
				}
				int buyamt = 0;
				try {
					buyamt = Integer.parseInt(allLines.get(i)[2]);
				}
				catch(NumberFormatException e) {
					badRead = true;
				}
				if(buyamt == 0) {
					badRead = true;
				}
				int price = -1;
				try {
					price = Integer.parseInt(allLines.get(i)[3]);
				}
				catch(NumberFormatException e) {
					badRead = true;
				}
				if(price <= 0) {
					badRead = true;
				}
				if(badRead) {
					break;
				}
				else {
					action.setStartTime(time);
					action.setTicker(allLines.get(i)[1]);
					action.setAmount(buyamt);
					action.setPrice(price);
					allActions.add(action);
				}
				
				
		}
		if(badRead) {
			System.out.println("Bad CSV File Formatting");
			allActions.clear();
			continue;
		}
		
		InValidFile = false;
		}

		System.out.println("The file " + fileName + " has been properly read!");
		
		System.out.println("Enter the starting balance");
		int balance = scan.nextInt();
		Stocks.start(balance, allActions);
		
		
		
		scan.close();
		}

}