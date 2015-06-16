// ========================================================================
// Copyright 1998-2005 Mort Bay Consulting Pty. Ltd.
// ------------------------------------------------------------------------
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at 
// http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ========================================================================

package org.mortbay.jetty.security;
import java.io.IOException;

import org.mortbay.log.Log;


/* ------------------------------------------------------------ */
/** Password utility class.
 *
 * This utility class gets a password or pass phrase either by:<PRE>
 *  + Password is set as a system property.
 *  + The password is prompted for and read from standard input
 *  + A program is run to get the password.
 * </pre>
 * Passwords that begin with OBF: are de obfuscated.
 * Passwords can be obfuscated by run org.mortbay.util.Password as a
 * main class.  Obfuscated password are required if a system needs
 * to recover the full password (eg. so that it may be passed to another
 * system). They are not secure, but prevent casual observation.
 * <p>
 * Passwords that begin with CRYPT: are oneway encrypted with
 * UnixCrypt. The real password cannot be retrieved, but comparisons
 * can be made to other passwords.  A Crypt can be generated by running
 * org.mortbay.util.UnixCrypt as a main class, passing password and
 * then the username. Checksum passwords are a secure(ish) way to
 * store passwords that only need to be checked rather
 * than recovered.  Note that it is not strong security - specially if
 * simple passwords are used.
 * 
 * @author Greg Wilkins (gregw)
 */
public class Password extends Credential
{
    public static final String __OBFUSCATE = "OBF:";

    private String _pw;
    
    /* ------------------------------------------------------------ */
    /** Constructor. 
     * @param password The String password.
     */
    public Password(String password)
    {
        _pw=password;
        
        // expand password
        while (_pw!=null && _pw.startsWith(__OBFUSCATE))
            _pw=deobfuscate(_pw);
    }    

    /* ------------------------------------------------------------ */
    public String toString()
    {
        return _pw;
    }
    
    /* ------------------------------------------------------------ */
    public String toStarString()
    {
        return "*****************************************************"
            .substring(0,_pw.length());
    }

    /* ------------------------------------------------------------ */
    public boolean check(Object credentials)
    {
	if (this == credentials)
	    return true;
	
        if (credentials instanceof Password)
            return credentials.equals(_pw);
        
        if (credentials instanceof String)
            return credentials.equals(_pw);
        
        if (credentials instanceof Credential)
            return ((Credential)credentials).check(_pw);
            
        return false;
    }

    /* ------------------------------------------------------------ */
    public boolean equals(Object o)
    {
	if (this == o)
	    return true;

        if (null == o)
            return false;

        if (o instanceof Password)
        {
            Password p=(Password)o;
            return p._pw == _pw || (null != _pw && _pw.equals(p._pw));
        }
        
        if (o instanceof String)
            return o.equals(_pw);
            
        return false;
    }

    /* ------------------------------------------------------------ */
    public int hashCode() {
        return null == _pw ? super.hashCode() : _pw.hashCode();
    }

    /* ------------------------------------------------------------ */
    public static String obfuscate(String s)
    {
        StringBuffer buf = new StringBuffer();
        byte[] b = s.getBytes();
        
        synchronized(buf)
        {
            buf.append(__OBFUSCATE);
            for (int i=0;i<b.length;i++)
            {
                byte b1 = b[i];
                byte b2 = b[s.length()-(i+1)];
                int i1= 127+b1+b2;
                int i2= 127+b1-b2;
                int i0=i1*256+i2;
                String x=Integer.toString(i0,36);

                switch(x.length())
                {
                  case 1:buf.append('0');
                  case 2:buf.append('0');
                  case 3:buf.append('0');
                  default:buf.append(x);
                }
            }
            return buf.toString();
        }
    }
    
    /* ------------------------------------------------------------ */
    public static String deobfuscate(String s)
    {
        if (s.startsWith(__OBFUSCATE))
            s=s.substring(4);
        
        byte[] b=new byte[s.length()/2];
        int l=0;
        for (int i=0;i<s.length();i+=4)
        {
            String x=s.substring(i,i+4);
            int i0 = Integer.parseInt(x,36);
            int i1=(i0/256);
            int i2=(i0%256);
            b[l++]=(byte)((i1+i2-254)/2);
        }

        return new String(b,0,l);
    }

    /* ------------------------------------------------------------ */
    /** Get a password.
     * A password is obtained by trying <UL>
     * <LI>Calling <Code>System.getProperty(realm,dft)</Code>
     * <LI>Prompting for a password
     * <LI>Using promptDft if nothing was entered.
     * </UL>
     * @param realm The realm name for the password, used as a SystemProperty name.
     * @param dft The default password.
     * @param promptDft The default to use if prompting for the password.
     * @return Password
     */
    public static Password getPassword(String realm,String dft, String promptDft)
    {
        String passwd=System.getProperty(realm,dft);
        if (passwd==null || passwd.length()==0)
        {
            try
            {
                System.out.print(realm+
                                 ((promptDft!=null && promptDft.length()>0)
                                  ?" [dft]":"")+" : ");
                System.out.flush();
                byte[] buf = new byte[512];
                int len=System.in.read(buf);
                if (len>0)
                    passwd=new String(buf,0,len).trim();
            }
            catch(IOException e)
            {
                Log.warn(Log.EXCEPTION,e);
            }
            if (passwd==null || passwd.length()==0)
                passwd=promptDft;
        }
        return new Password(passwd);
    }
    
    
    /* ------------------------------------------------------------ */
    /** 
     * @param arg 
     */
    public static void main(String[] arg)
    {
        if (arg.length!=1 && arg.length!=2 )
        {
            System.err.println("Usage - java org.mortbay.jetty.security.Password [<user>] <password>");
            System.err.println("If the password is ?, the user will be prompted for the password");
            System.exit(1);
        }
        String p=arg[arg.length==1?0:1];
        Password pw = "?".equals(p)?new Password(p):new Password(p);
        System.err.println(pw.toString());
        System.err.println(obfuscate(pw.toString()));
        System.err.println(Credential.MD5.digest(p));
        if (arg.length==2)
            System.err.println(Credential.Crypt.crypt(arg[0],pw.toString()));
    }    
}


