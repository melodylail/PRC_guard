/**
* common function for ajax success handler
* dependency: i_settings.js, i_util.js
*/


function jsGetRequest(url, on_success, on_error, timeout, on_before, on_complete) {
	jsAjaxRequest('GET', url, null, on_success, on_error, timeout, on_before, on_complete);
}
function jsPostRequest(url, data, on_success, on_error, timeout, on_before, on_complete) {
	jsAjaxRequest('POST', url, data, on_success, on_error, timeout, on_before, on_complete);
}

function jsDelRequest(url, on_success, on_error, timeout, on_before, on_complete) {
	jsAjaxRequest('DELETE', url, false, on_success, on_error, timeout, on_before, on_complete);
}

function jsPutRequest(url, data, on_success, on_error, timeout, on_before, on_complete) {
	jsAjaxRequest('PUT', url, data, on_success, on_error, timeout, on_before, on_complete);
}

function jsAjaxRequest(method, url, data, on_success, on_error, timeout, on_before, on_complete) {	
    var request = {
        type: method,
        url: TMMS_WEB_ROOT+url,
        data: data ? $.toJSON(data) : '',
        beforeSend: function (XHR, setting) {
            if (on_before) {
                on_before(XHR, setting);
            }
        },
        complete: function (XHR, setting) {
            if (on_complete) {
                on_complete(XHR, setting);
            }
        },
        error: function (XHR, textStatus, errorThrown) {
        	onAjaxError(XHR, textStatus, errorThrown, on_error,url);
        },
        success: function (response, textStatus) {
            onAjaxSuccess(response, textStatus, on_success);
        },
        headers: {
            "Content-type": "application/json",
            "Accept": "application/json",
			"Cache-Control":"no-cache",
            "X-CSRFToken": GetCookie('csrftoken')
        },
		cache:false,
	    dataType:"json"
    };
    
    if (timeout) {
        request["timeout"] = timeout;
    }

    return $.ajax(request);
}

function onAjaxSuccess(response, textStatus, on_success) {
    if (!response) {
        response = {};
    }

    if ("string" == typeof (response)) {
        try {
            response = $.secureEvalJSON(response);
        } catch (e) {
        	alert("<TREND_L10N>GLOBAL_JS_SERVER_ERROR</TREND_L10N>");
        	return;
        }
    }

    if (on_success) {
        on_success(response);
    }
}
function onAjaxError(XHR, textStatus, errorThrown, on_error,url) {
	var response = XHR.responseText;
	if(XHR.status==403 && url.indexOf("cfg/license/?from=index")!=-1){
		window.parent.location.href="../login.htm?error_id=403";
		return;
	}
	if ("string" == typeof (response) && response.length > 0) {
		try {
			response = $.secureEvalJSON(response);
		} catch (e) {
        	alert("<TREND_L10N>GLOBAL_JS_SERVER_ERROR</TREND_L10N>");
			return;
	    }
    }
    else {
    	response = {"detail": errorThrown}
    }
	if(url.indexOf("account/logout/")!=-1){
		window.parent.location.href="../login.htm?error_id=logout_fail";
		return;
	}
	
	if(XHR.status==403){
		var filename=getFileName(this.location.href).toLowerCase();
		if(filename.indexOf("login.htm")==-1){
			window.parent.location.href="../login.htm?error_id=403";
		}else{
        	alert("<TREND_L10N>GLOBAL_JS_SERVER_ERROR</TREND_L10N>");
		}
		return;
	}
	if(XHR.status!=400 && XHR.status!=0){
        alert("<TREND_L10N>GLOBAL_JS_SERVER_ERROR</TREND_L10N>");
		return;
	}
    
    if (on_error && XHR.status!=0) {	
		on_error(response,XHR.status);  //XHR.status  400
	}
}
