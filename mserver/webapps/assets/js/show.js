"use strict";

var refreshSeconds = 60 * 60 * 6; // 6 hours
setTimeout("location.reload()", refreshSeconds * 1000);

var interval = null;

(function(window) {
    var isChrome = navigator.userAgent.toLowerCase().indexOf('chrome') > -1;

    var settings = {
        consoleTableRows: 16,
    };

    /*
     * HTML Interface
     */

    var colorizer = d3.scale.category20();

    /**************
     * Internal API
     */

    function spanWrap(content, classes) {
        // Returns the content wrapped in the span
        return '<span class="' + (classes ? classes.join(" ") : "") + '">' +
            content + '</span>'
    }

    function dist(x1, y1, x2, y2) {
        // Returns the distance between two points
        return Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
    }

    function rgbaString(c, a) {
        // Helper to get the color as an rgba string
        return "rgba(" + c.r + "," + c.g + "," + c.b + "," + a + ")";
    }

    var getID = (function() {
        // Generate unique enough IDs
        var i = 0;
        return function() {
            return i++;
        }
    })();

    function isNumber(n) {
        // Returns true if n is a number
        return !isNaN(parseFloat(n)) && isFinite(n);
    }

    /*
     * Stats the state machine
     */
    function Stats(params) {
        this.state = params.state;

        this.insert = function(incoming) {
            // Insert a new item, updating the state. params.insert should mutate
            params.insert(incoming, this.state);
        };

        this.data = function() {
            // Get the data as a list
            if (params.data) return params.data(this.state); else this.state;
        }

        this.redraw = function() {
            if (params.redraw) {
                params.redraw()
            } 
        }
    }

    var statsManager = {
        insert: function(incoming) {
            for (var i = 0; i < this.stats.length; ++i) {
                this.stats[i].insert(incoming);
            }
        },

        redraw: function() {
            for (var i = 0; i < this.stats.length; i++) {
                this.stats[i].redraw();
            }
        },

        stats: [
            new Stats({
                // #console stats manager
                state: [],

                insert: function(incoming, state) {
                    state.push(incoming);
                    while (state.length > settings.consoleTableRows) {
                        state.shift();
                    }
                    return state;
                },

                redraw: function() {
                    var that = this;
                    var rows = d3.select("#events-data").selectAll("tr.srow")
                        .data(this.state, function(d) { return d.id });

                    rows.enter().append("tr")
                        .style("color", function(d) { return colorizer(d.id); })
                        .attr("class", "srow")
                        .style("font-size", "18px");
                    rows.exit().remove();

                    var cols = rows.selectAll("td")
                        .data(function(d) {
                            return [d.time,
                                    d.user,
                                    d.name,
                                    d.room,
                                    d.msg]; });

                    cols.enter().append("td")
                        .html(function(d) { return d; });
                    cols.exit().remove();
                }

            })
        ]};


    function queryLatestReports(){
        $.ajax({
            type: "GET",
            url: "http://192.168.245.149/console/report/query_latest.bin", 
            contentType: "application/json; charset=utf-8",
            dataType: "json",
           
            error: function(XMLHttpRequest, textStatus, errorThrown) { 
              
            }, 

            success: function(data){
                handleResult(data);
                
            } // success
          }); // ajax
    }

    function handleResult(data){
        var jsonObj = eval(data)

        if(data.error_code == 1)
        {
            var reports = jsonObj.data;
            
            jQuery.each(reports, function(index, row) {
                var datum = {};
                datum.msg = row.msg;
                datum.time = row.time;
                datum.user = row.user;
                datum.name = row.name;
                datum.room = row.room;
                datum.id = getID();
                
                statsManager.insert(datum);
                statsManager.redraw();           
            });
        }


    }

    interval = setInterval(function () { queryLatestReports(); }, 3 * 1000);

})(window);


$(window).unload(function(){
    clearInterval(interval);
});
