//========================================================================
//$Id: Attributes.java,v 1.3 2005/11/14 17:45:52 gregwilkins Exp $
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

package org.mortbay.util;

import java.util.Enumeration;

/* ------------------------------------------------------------ */
/** Attributes.
 * Interface commonly used for storing attributes.
 * @author gregw
 *
 */
public interface Attributes
{
    public void removeAttribute(String name);
    public void setAttribute(String name, Object attribute);
    public Object getAttribute(String name);
    public Enumeration getAttributeNames();
    public void clearAttributes();
}
