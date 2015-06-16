package com.king.server.util;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

public class XMLParser {
	
	private File file = null;
	private Document doc = null;
	private Element root = null;
	
	public XMLParser(File file) {
		this.file = file;
	}
	
	public void init(){
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = null;
		try {
			builder = factory.newDocumentBuilder();
		} catch (ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		try {
			doc = builder.parse(this.file);
		} catch (SAXException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		root = doc.getDocumentElement();
	}
	
	public String getElementAttribute(Element element, String attrName) {
		return element.getAttribute(attrName);
	}
	
	public String getRootAttribute(String attrName) {
		return root.getAttribute(attrName);
	}
	
	public List<Element> getElement(String name) {
		List<Element> elements = new ArrayList<Element>();
		NodeList nodeList = root.getElementsByTagName(name);
		for (int i = 0; i <nodeList.getLength(); i ++) {
			Node node = nodeList.item(i);
			elements.add((Element) node);
		}
		return elements;
	}

}
