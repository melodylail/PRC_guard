//========================================================================
//$Id: PostConstructCallback.java 1540 2007-01-19 12:24:10Z janb $
//Copyright 2006 Mort Bay Consulting Pty. Ltd.
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

package org.mortbay.jetty.plus.annotation;

import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

import org.mortbay.log.Log;

/**
 * PostConstructCallback
 *
 *
 */
public class PostConstructCallback extends LifeCycleCallback
{

    /** 
     * Commons Annotation Specification section 2.5
     *  - no params
     *  - must be void return 
     *  - no checked exceptions
     *  - cannot be static
     * @see org.mortbay.jetty.plus.annotation.LifeCycleCallback#validate(java.lang.Class, java.lang.reflect.Method)
     */
    public void validate(Class clazz, Method method)
    {
        if (method.getExceptionTypes().length > 0)
            throw new IllegalArgumentException(clazz.getName()+"."+method.getName()+ " cannot not throw a checked exception");
        
        if (!method.getReturnType().equals(Void.TYPE))
            throw new IllegalArgumentException(clazz.getName()+"."+method.getName()+ " cannot not have a return type");
        
        if (Modifier.isStatic(method.getModifiers()))
            throw new IllegalArgumentException(clazz.getName()+"."+method.getName()+ " cannot be static");
    }
    
    
    public void callback (Object instance)
    throws Exception
    {
        super.callback(instance);
    }

    public boolean equals (Object o)
    {
        if (super.equals(o) && (o instanceof PostConstructCallback))
            return true;
        return false;
    }
}
