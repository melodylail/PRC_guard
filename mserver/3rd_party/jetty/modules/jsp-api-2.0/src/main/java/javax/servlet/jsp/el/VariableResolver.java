/**
 *
 * Copyright 2003-2004 The Apache Software Foundation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

//
// This source code implements specifications defined by the Java
// Community Process. In order to remain compliant with the specification
// DO NOT add / change / or delete method signatures!
//

package javax.servlet.jsp.el;

/**
 * <p>This class is used to customize the way an ExpressionEvaluator resolves
 * variable references at evaluation time.  For example, instances of this class can
 * implement their own variable lookup mechanisms, or introduce the
 * notion of "implicit variables" which override any other variables.
 * An instance of this class should be passed when evaluating
 * an expression.</p>
 *
 * <p>An instance of this class includes the context against which resolution
 * will happen</p>
 *
 * @since 2.0
 */
public interface VariableResolver
{
  //-------------------------------------
  /**
   * Resolves the specified variable.
   * Returns null if the variable is not found.
   * 
   * @param pName the name of the variable to resolve
   * @return the result of the variable resolution
   *
   * @throws ELException if a failure occurred while trying to resolve
   *     the given variable
   **/
  public Object resolveVariable (String pName)
    throws ELException;
					
  //-------------------------------------
}
