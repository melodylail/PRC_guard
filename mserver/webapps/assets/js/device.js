
var device_template='<tr> \
                        <td>{0}</td> \
                        <td>{1}</td> \
                        <td>{2}</td> \
                        <td>{3}</td> \
                        <td>{4}</td> \
                        <td>{5}</td> \
                        <td>{6}</td> \
                     </tr>';

function queryAllDevices(){
	$.ajax({
        type: "GET",
        url: "http://192.168.245.149/device/query_all.bin", 
        contentType: "application/json; charset=utf-8",
        dataType: "json",
       
        error: function(XMLHttpRequest, textStatus, errorThrown) { 
          
        }, 
        success: function(data){
          handleDevices(data)
            
        } // success
      }); // ajax
}

function handleDevices(data){

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
        var devices = jsonObj.data;
        var devicesHtml="";
        
        jQuery.each(devices, function(index, row) {
            var status = "在线";

            if(row.status == 1){
                status = "在线";
            }
            else{
                status = "离线";
            }
			var record = String.format(device_template, row.id, row.name, row.type, status, row.channel, row.netinferface, row.lastlivetime);
			devicesHtml = devicesHtml + record;
			
		});
		
		$("#devices_table tbody").html(devicesHtml);
    }
};

