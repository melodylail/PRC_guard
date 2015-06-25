
/*$(document).ready(function(){

  queryAllReports();
  
});*/


function queryAllReports(){
        $("#report_table").dataTable({
        "processing":true,
        "serverSide":true,
        "bFilter": false,
        "bSort": false,
        "bLengthChange": false,
        "ajax":{
            type: "GET",
            url: "http://192.168.245.149/console/report/query_all.bin?user=mike", 
            contentType: "application/json; charset=utf-8",
            dataType: "json"},
        "columns": [
        { "data": "time" },
        { "data": "user" },
        { "data": "name" },
        { "data": "room" },
        { "data": "msg" }
    ]
    });
}

function queryReportsByUser(name){
    
    var table = $('#report_table').DataTable();
 
    table.clear();
    
    $("#report_table").dataTable({
        "processing":true,
        "serverSide":true,
        "bFilter": false,
        "bSort": false,
        "bLengthChange": false,
        "bDestroy": true,
        "ajax":{
            type: "GET",
            url: "http://192.168.245.149/console/report/query_reports.bin?account=" + name, 
            contentType: "application/json; charset=utf-8",
            dataType: "json"},
        "columns": [
        { "data": "time" },
        { "data": "user" },
        { "data": "name" },
        { "data": "room" },
        { "data": "msg" }
        ]
    });
}

function queryAllReports11(){
	$.ajax({
        type: "GET",
        url: "http://192.168.245.149/console/report/query_all.bin", 
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

        $("#report_table").dataTable({
            data: reports,
            "columns": [
            { "data": "time" },
            { "data": "user" },
            { "data": "name" },
            { "data": "room" },
            { "data": "msg" }
        ]
        });

    }
};