/**
* common function for ajax success handler
* dependency: i_settings.js
*/
function GoLogin(){
	var topWin = window;
	while(topWin!=topWin.parent)
		topWin = topWin.parent;
	topWin.location.href = "../login.htm";
}
function setCookie(NameOfCookie, value, expiredays){ 
	var ExpireDate = new Date (); 
	ExpireDate.setTime(ExpireDate.getTime() + (expiredays * 24 * 3600 * 1000)); 
	document.cookie = NameOfCookie + "=" + escape(value) +  ((expiredays == null) ? "" : "; expires=" + ExpireDate.toGMTString()+"; path=/"); 
}
function GetCookie(name) {
	var arg = name + "=";
	var alen = arg.length;
	var clen = document.cookie.length;
	var i = 0;
	while (i < clen) {
		var j = i + alen;
		if (document.cookie.substring(i, j) == arg)
		return GetCookieVal (j);
		i = document.cookie.indexOf(" ", i) + 1;
		if (i == 0) break;
	}
	return null;
}

function GetCookieVal(offset) {
	var endstr = document.cookie.indexOf (";", offset);
	if (endstr == -1) {
		endstr = document.cookie.length;
	}
	return unescape(document.cookie.substring(offset, endstr));
}

function DeleteCookie(name) {
	var exp = new Date();
	exp.setTime(exp.getTime() - 1);
	var cval=GetCookie(name);
	if(cval!=null) document.cookie= name + "="+cval+";expires="+exp.toGMTString();
}
function deleteCookie(name){
	var exp = new Date();
	exp.setTime(exp.getTime() - 24 * 3600*1000);
	var cval=GetCookie(name);
	if(cval!=null) document.cookie= name + "="+cval+";expires="+exp.toGMTString()+"; path=/";
}

//deep copy for js array and object
function clone(subject) {
	//just process array and object
	if(typeof subject != 'object') {     
		return subject;     
	}     
    
	//array need special process     
	if(Object.prototype.toString.call(subject) === '[object Array]') {     
		var array = [];     
		for(var i = 0; i < subject.length; i++) {     
			if(typeof subject[i] === 'object') {     
				array[i] = clone(subject[i]);     
			}
			else {     
				array[i] = subject[i];     
			}     
		}     
		return array;     
	}     
    
	//process object type    
	var copy = {};     
	for(var name in subject) {     
		copy[name] = clone(subject[name]);     
	}     
	return copy;                                                     
}    


/* jsIsEmailValid: check if the email address is valid
 * email: email address
 */
function jsIsEmailValid(email) {	
	if (!email) {
		return false;
	}
    var pattern = new RegExp("^(\"[^\"]*\" )?([\\w.-]+@[a-zA-Z0-9.-]+$)");
    var result = pattern.exec(email);
    return result != null;
}

function getFileName(url) {
	var pos = url.lastIndexOf("/");
	if(pos == -1) {
	   pos = url.lastIndexOf("\\")
	}
	var filename = url.substr(pos +1)
	return filename;
};

$(function() {	
	var username = GetCookie(I_SESSION_KEY);
	if(!username || username == '' ){
		var filename=getFileName(this.location.href).toLowerCase();
		if(filename.indexOf("login.htm")==-1){
			if(filename.indexOf("appwrap.htm")>-1){
				window.parent.location.href="../login.htm?error_id=403&appwrap=1";
			}else{
				window.parent.location.href="../login.htm?error_id=403";
			}
		}
	}else{
		$('.headerInformation .loginName .board').text(username);
	}
	
	$('li.logoff').click(function(){
		var request = {"action":"logout"};
		jsPostRequest('account/logout/', request,function(response){			
			parent.location.href="../login.htm"
			},function(){
			parent.location.href="../login.htm"
			});
	});
	/*
	$('h1.productBanner a').click(function(){
		$.get('/api/v1/account/userlogzip/', function(response){
			location.href="../users/"+response;
		});
	});
	*/
	
}); 

/* 
	common function to set a value to a html element
*/
function set_value_by_id(element_id, value) {
	var element = $("#" + element_id);
	if (element.attr("type") != null && element.attr("type") == "checkbox") {
		element.attr("checked", value == "True");
	}
	else {
		element.val(value);
	}
}

/* 
	common function to get a value from a html element
*/		
function get_value_by_id(element_id) {
	var element = $("#" + element_id);
	if (element.attr("type") != null && element.attr("type") == "checkbox") {
		return element.attr("checked") ? "True" : "False";
	}
	else {
		return element.val();
	}
}

function to_locale_time(date) {
	//var d="2013-08-20T09:55:57+00:00";
	if(!date) return '-';
	if(isNaN(Date.parse(date))){
		date=date.replace(/-/g,"\/").replace("T"," ").replace("+00:00","Z");
	}
	var date=Date.parse(date);
	var d = new Date(date);
    var pad = function(n, c){
        if((n = n + "").length < c){
            return new Array(++c - n.length).join("0") + n;
        } else {
            return n;
        }
    };
	return d.getFullYear()+'/'+pad((d.getMonth()+1),2)+'/'+pad(d.getDate(),2)+' '+pad(d.getHours(),2)+':'+pad(d.getMinutes(),2)+':'+pad(d.getSeconds(),2);
}

/**
 * The same to php date function.
 * @param  {string} format    Y-m-d 
 * @param  {int}    timestamp  The length is 10 numbers. default is current time
 * @return {string}           
 * Exp: alert(date('Y-m-d','1350052653')); date('Y-m-d');
 */
function date(format, timestamp){
    var a, jsdate=((timestamp) ? new Date(timestamp*1000) : new Date());
    var pad = function(n, c){
        if((n = n + "").length < c){
            return new Array(++c - n.length).join("0") + n;
        } else {
            return n;
        }
    };
    var txt_weekdays = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];
    var txt_ordin = {1:"st", 2:"nd", 3:"rd", 21:"st", 22:"nd", 23:"rd", 31:"st"};
    var txt_months = ["", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];
    var f = {
        // Day
        d: function(){return pad(f.j(), 2)},
        D: function(){return f.l().substr(0,3)},
        j: function(){return jsdate.getDate()},
        l: function(){return txt_weekdays[f.w()]},
        N: function(){return f.w() + 1},
        S: function(){return txt_ordin[f.j()] ? txt_ordin[f.j()] : 'th'},
        w: function(){return jsdate.getDay()},
        z: function(){return (jsdate - new Date(jsdate.getFullYear() + "/1/1")) / 864e5 >> 0},
       
        // Week
        W: function(){
            var a = f.z(), b = 364 + f.L() - a;
            var nd2, nd = (new Date(jsdate.getFullYear() + "/1/1").getDay() || 7) - 1;
            if(b <= 2 && ((jsdate.getDay() || 7) - 1) <= 2 - b){
                return 1;
            } else{
                if(a <= 2 && nd >= 4 && a >= (6 - nd)){
                    nd2 = new Date(jsdate.getFullYear() - 1 + "/12/31");
                    return date("W", Math.round(nd2.getTime()/1000));
                } else{
                    return (1 + (nd <= 3 ? ((a + nd) / 7) : (a - (7 - nd)) / 7) >> 0);
                }
            }
        },
       
        // Month
        F: function(){return txt_months[f.n()]},
        m: function(){return pad(f.n(), 2)},
        M: function(){return f.F().substr(0,3)},
        n: function(){return jsdate.getMonth() + 1},
        t: function(){
            var n;
            if( (n = jsdate.getMonth() + 1) == 2 ){
                return 28 + f.L();
            } else{
                if( n & 1 && n < 8 || !(n & 1) && n > 7 ){
                    return 31;
                } else{
                    return 30;
                }
            }
        },
       
        // Year
        L: function(){var y = f.Y();return (!(y & 3) && (y % 1e2 || !(y % 4e2))) ? 1 : 0},
        //o not supported yet
        Y: function(){return jsdate.getFullYear()},
        y: function(){return (jsdate.getFullYear() + "").slice(2)},
       
        // Time
        a: function(){return jsdate.getHours() > 11 ? "pm" : "am"},
        A: function(){return f.a().toUpperCase()},
        B: function(){
            // peter paul koch:
            var off = (jsdate.getTimezoneOffset() + 60)*60;
            var theSeconds = (jsdate.getHours() * 3600) + (jsdate.getMinutes() * 60) + jsdate.getSeconds() + off;
            var beat = Math.floor(theSeconds/86.4);
            if (beat > 1000) beat -= 1000;
            if (beat < 0) beat += 1000;
            if ((String(beat)).length == 1) beat = "00"+beat;
            if ((String(beat)).length == 2) beat = "0"+beat;
            return beat;
        },
        g: function(){return jsdate.getHours() % 12 || 12},
        G: function(){return jsdate.getHours()},
        h: function(){return pad(f.g(), 2)},
        H: function(){return pad(jsdate.getHours(), 2)},
        i: function(){return pad(jsdate.getMinutes(), 2)},
        s: function(){return pad(jsdate.getSeconds(), 2)},
        //u not supported yet
       
        // Timezone
        //e not supported yet
        //I not supported yet
        O: function(){
            var t = pad(Math.abs(jsdate.getTimezoneOffset()/60*100), 4);
            if (jsdate.getTimezoneOffset() > 0) t = "-" + t; else t = "+" + t;
            return t;
        },
        P: function(){var O = f.O();return (O.substr(0, 3) + ":" + O.substr(3, 2))},
        //T not supported yet
        //Z not supported yet
       
        // Full Date/Time
        c: function(){return f.Y() + "-" + f.m() + "-" + f.d() + "T" + f.h() + ":" + f.i() + ":" + f.s() + f.P()},
        //r not supported yet
        U: function(){return Math.round(jsdate.getTime()/1000)}
    };
       
    return format.replace(/[\\]?([a-zA-Z])/g, function(t, s){
        if( t!=s ){
            // escaped
            ret = s;
        } else if( f[s] ){
            // a date function exists
            ret = f[s]();
        } else{
            // nothing special
            ret = s;
        }
        return ret;
    });
}

//replace string
function replaceRevStr(str){
        var str=str;
		if(str=="") return "";
		//var regObj = {"&lt;":"<","&gt;":">"};
		var regObj = {"<":"&lt;",">":"&gt;"};
		for(var reg in regObj){
            str = str.replace(new RegExp(reg,"g"),regObj[reg]);
        }
        return str;
}
