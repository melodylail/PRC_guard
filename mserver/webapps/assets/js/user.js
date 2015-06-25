
var user_template='<tr> \
                        <td>{0}</td> \
                        <td>{1}</td> \
                        <td>{2}</td> \
                        <td>{3}</td> \
                        <td><a href="user_edit.html?user={4}">编辑</a> | <a href="user_list.html" onclick="deleteUser({5}); return false;">删除</a></td> \
                     </tr>';

function queryAllUsers(){
	$.ajax({
        type: "GET",
        url: "http://192.168.245.149/console/user/list_all.bin", 
        contentType: "application/json; charset=utf-8",
        dataType: "json",
       
        error: function(XMLHttpRequest, textStatus, errorThrown) { 
          
        }, 
        success: function(data){
          handleResult(data)
            
        } // success
      }); // ajax
}

function handleResult(data){

    String.format = function(src) {
        if (arguments.length == 0) return null;
        var args = Array.prototype.slice.call(arguments, 1);
        
        return src.replace(/\{(\d+)\}/g, function(m, i) {
            return args[i];
        });
    };
    
	var jsonObj = eval(data)

	if(data.error_code == 1)
    {
        var users = jsonObj.data;
        var usersHtml="";
        
        jQuery.each(users, function(index, row) {
            var rolename = "管理员";

            if(row.role == 1){
                rolename = "管理员";
            }
            else if(row.role == 2){
                rolename = "操作员";
            }
            else{
                rolename = "浏览用户";
            }
			var record = String.format(user_template, row.user_name, rolename, row.email, row.last_modified_time, row.user_name, row.user_name);
			usersHtml = usersHtml + record;
			
		});
		
		$("#users_table tbody").html(usersHtml);
    }
};

function saveUser() {     
    var username = $("#form_add_user #user_name").val(); // get username
    var password = $("#form_add_user #password").val(); // get password
    var role = $("#form_add_user #role").val(); 
    var email = $("#form_add_user #email").val(); 

    if (username && password && email) { // values are not empty
      var strjson=JSON.stringify({'user_name':username,'password':password, 'email':email, 'role':role});
      
      $.ajax({
        type: "POST",
        url: "http://192.168.245.149/console/user/add.bin", // URL of the Perl script
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        // send username and password as parameters to the Perl script
        data: strjson,
        // script call was *not* successful
        error: function(XMLHttpRequest, textStatus, errorThrown) { 
          alert("添加用户失败， responseText: " + XMLHttpRequest.responseText 
            + ", textStatus: " + textStatus 
            + ", errorThrown: " + errorThrown);
        }, // error 
        // script call was successful 
        // data contains the JSON values returned by the Perl script 
        success: function(data){
            if(data.error_code == 1)
            {
                window.location.href = "user_list.html";
            }
            else
            {
                alert("添加用户失败: " + data.message);
            }
        } // success
      }); // ajax
    } // if
    else {
      alert("请输入用户名/密码/邮箱");
    } // else

};


function getParameterByName(name) {
    name = name.replace(/[\[]/, "\\[").replace(/[\]]/, "\\]");
    var regex = new RegExp("[\\?&]" + name + "=([^&#]*)"),
        results = regex.exec(location.search);

    return results === null ? "" : decodeURIComponent(results[1].replace(/\+/g, " "));
};

function queryUserByName(name){
    $.ajax({
        type: "GET",
        url: "http://192.168.245.149/console/user/get_one.bin?account="+name, 
        contentType: "application/json; charset=utf-8",
        dataType: "json",
       
        error: function(XMLHttpRequest, textStatus, errorThrown) { 
          
        }, 
        success: function(data){
          handleOneUserResult(data)
            
        } // success
      }); // ajax
}

function handleOneUserResult(data){
    
    var jsonObj = eval(data)

    if(data.error_code == 1)
    {
        var user = jsonObj.data;
        
        $("#form_edit_user #user_name").val(user.user_name); // get username
        $("#form_edit_user #password").val(user.password); // get password
        $("#form_edit_user #role").val(user.role); 
        $("#form_edit_user #email").val(user.email); 
    }
};

function editUser() {     
    var username = $("#form_edit_user #user_name").val(); // get username
    var password = $("#form_edit_user #password").val(); // get password
    var role = $("#form_edit_user #role").val(); 
    var email = $("#form_edit_user #email").val(); 

    if (username && password && email) { // values are not empty
      var strjson=JSON.stringify({'user_name':username,'password':password, 'email':email, 'role':role});
      
      $.ajax({
        type: "POST",
        url: "http://192.168.245.149/console/user/edit.bin", // URL of the Perl script
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        // send username and password as parameters to the Perl script
        data: strjson,
        // script call was *not* successful
        error: function(XMLHttpRequest, textStatus, errorThrown) { 
          alert("添加用户失败， responseText: " + XMLHttpRequest.responseText 
            + ", textStatus: " + textStatus 
            + ", errorThrown: " + errorThrown);
        }, // error 
        // script call was successful 
        // data contains the JSON values returned by the Perl script 
        success: function(data){
            if(data.error_code == 1)
            {
                window.location.href = "user_list.html";
            }
            else
            {
                alert("修改用户失败: " + data.message);
            }
        } // success
      }); // ajax
    } // if
    else {
      alert("请输入用户名/密码/邮箱");
    } // else

};

function deleteUser(name){
    $.ajax({
        type: "POST",
        url: "http://192.168.245.149/console/user/delete.bin?account="+name, 
        contentType: "application/json; charset=utf-8",
        dataType: "json",
       
        error: function(XMLHttpRequest, textStatus, errorThrown) { 
          
        }, 
        success: function(data){
            if(data.error_code == 1)
            {
                window.location.href = "user_list.html";
            }
            else
            {
                alert("删除用户失败: " + data.message);
            }
            
        } // success
      }); // ajax
}