
/*$(document).ready(function(){

  queryAllReports();
  
});*/

function queryAllReports(){
	$.ajax({
        type: "GET",
        url: "/query_all.bin", 
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
        var reports = jsonObj.data;
        var reportHtml="";
        
        jQuery.each(reports, function(index, row) {
			var report = String.format(report_template, row.user_name, row.content, row.picture_url, row.picture_url, row.voice_url, row.time);
			reportHtml = reportHtml + report;
			
		});
		
		jQuery(".report_table").html(reportHtml);
    }
};