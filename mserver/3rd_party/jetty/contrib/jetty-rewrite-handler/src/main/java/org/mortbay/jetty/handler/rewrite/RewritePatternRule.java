//========================================================================
//$Id: RewritePatternRule.java 966 2008-04-17 13:53:44Z gregw $
//Copyright 2004-2005 Mort Bay Consulting Pty. Ltd.
//------------------------------------------------------------------------
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at 
//http://www.apache.org/licenses/LICENSE-2.0
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.
//========================================================================
package org.mortbay.jetty.handler.rewrite;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.mortbay.jetty.servlet.PathMap;
import org.mortbay.util.URIUtil;

/**
 * Rewrite the URI by replacing the matched {@link PathMap} path with a fixed string. 
 */
public class RewritePatternRule extends PatternRule
{
    private String _replacement;

    /* ------------------------------------------------------------ */
    public RewritePatternRule()
    {
        _handling = false;
        _terminating = false;
    }

    /* ------------------------------------------------------------ */
    /**
     * Whenever a match is found, it replaces with this value.
     * 
     * @param value the replacement string.
     */
    public void setReplacement(String value)
    {
        _replacement = value;
    }

    /* ------------------------------------------------------------ */
    /*
     * (non-Javadoc)
     * @see org.mortbay.jetty.handler.rules.RuleBase#apply(javax.servlet.http.HttpServletRequest, javax.servlet.http.HttpServletResponse)
     */
    public String apply(String target, HttpServletRequest request, HttpServletResponse response) throws IOException
    {
        target = URIUtil.addPaths(_replacement, PathMap.pathInfo(_pattern,target));   
        return target;
    }

    /* ------------------------------------------------------------ */
    /**
     * Returns the replacement string.
     */
    public String toString()
    {
        return super.toString()+"["+_replacement+"]";
    }
}
