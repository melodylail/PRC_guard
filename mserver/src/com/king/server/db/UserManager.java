package com.king.server.db;

import java.util.ArrayList;

import com.king.server.data.User;

public interface UserManager {	
	public boolean addUser(User usr);	
	public User findUser(String userName);
	public boolean verify(String userName, String password);	
	public boolean changeEmail(String userName, String email);
	public ArrayList<User> getAllUser();
}
