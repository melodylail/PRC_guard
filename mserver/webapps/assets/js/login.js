$(document).ready(function(){
  $("form#loginForm").submit(function() { // loginForm is submitted
    
    var username = $("input[name='username']").val(); // get username
    var password = $("input[name='password']").val(); // get password

    if (username && password) { // values are not empty
      var strjson=JSON.stringify({'user_name':username,'password':password});
      
      $.ajax({
        type: "POST",
        url: "/console/user/login.bin", // URL of the Perl script
        contentType: "application/json; charset=utf-8",
        dataType: "json",
        // send username and password as parameters to the Perl script
        data: strjson,
        // script call was *not* successful
        error: function(XMLHttpRequest, textStatus, errorThrown) { 
          alert("登录失败， responseText: " + XMLHttpRequest.responseText 
            + ", textStatus: " + textStatus 
            + ", errorThrown: " + errorThrown);
        }, // error 
        // script call was successful 
        // data contains the JSON values returned by the Perl script 
        success: function(data){
            if(data.error_code == 1)
            {
	            window.location.href = "index.html";
            }
            else
            {
	            alert("登录失败: " + data.message);
            }
        } // success
      }); // ajax
    } // if
    else {
      alert("请输入用户名和密码");
    } // else
    $('div#loginResult').fadeIn();
    return false;
  });
});