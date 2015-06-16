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

package javax.servlet.jsp.tagext;

import javax.servlet.jsp.JspException;
import javax.servlet.jsp.JspWriter;

/**
 * A base class for defining tag handlers implementing BodyTag.
 *
 * <p>
 * The BodyTagSupport class implements the BodyTag interface and adds
 * additional convenience methods including getter methods for the
 * bodyContent property and methods to get at the previous out JspWriter.
 *
 * <p>
 * Many tag handlers will extend BodyTagSupport and only redefine a
 * few methods.
 */

public class BodyTagSupport extends TagSupport implements BodyTag {

    /**
     * Default constructor, all subclasses are required to only define
     * a public constructor with the same signature, and to call the
     * superclass constructor.
     *
     * This constructor is called by the code generated by the JSP
     * translator.
     */

    public BodyTagSupport() {
	super();
    }

    /**
     * Default processing of the start tag returning EVAL_BODY_BUFFERED.
     *
     * @return EVAL_BODY_BUFFERED
     * @throws JspException if an error occurred while processing this tag
     * @see BodyTag#doStartTag
     */
 
    public int doStartTag() throws JspException {
        return EVAL_BODY_BUFFERED;
    }


    /**
     * Default processing of the end tag returning EVAL_PAGE.
     *
     * @return EVAL_PAGE
     * @throws JspException if an error occurred while processing this tag
     * @see Tag#doEndTag
     */

    public int doEndTag() throws JspException {
	return super.doEndTag();
    }


    // Actions related to body evaluation

    /**
     * Prepare for evaluation of the body: stash the bodyContent away.
     *
     * @param b the BodyContent
     * @see #doAfterBody
     * @see #doInitBody()
     * @see BodyTag#setBodyContent
     */

    public void setBodyContent(BodyContent b) {
	this.bodyContent = b;
    }


    /**
     * Prepare for evaluation of the body just before the first body evaluation:
     * no action.
     *
     * @throws JspException if an error occurred while processing this tag
     * @see #setBodyContent
     * @see #doAfterBody
     * @see BodyTag#doInitBody
     */

    public void doInitBody() throws JspException {
    }


    /**
     * After the body evaluation: do not reevaluate and continue with the page.
     * By default nothing is done with the bodyContent data (if any).
     *
     * @return SKIP_BODY
     * @throws JspException if an error occurred while processing this tag
     * @see #doInitBody
     * @see BodyTag#doAfterBody
     */

    public int doAfterBody() throws JspException {
 	return SKIP_BODY;
    }


    /**
     * Release state.
     *
     * @see Tag#release
     */

    public void release() {
	bodyContent = null;

	super.release();
    }

    /**
     * Get current bodyContent.
     *
     * @return the body _content.
     */
    
    public BodyContent getBodyContent() {
	return bodyContent;
    }


    /**
     * Get surrounding out JspWriter.
     *
     * @return the enclosing JspWriter, from the bodyContent.
     */

    public JspWriter getPreviousOut() {
	return bodyContent.getEnclosingWriter();
    }

    // protected fields

    /**
     * The current BodyContent for this BodyTag.
     */
    protected BodyContent   bodyContent;
}
