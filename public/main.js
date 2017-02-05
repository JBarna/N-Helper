(function(){
    
    var startTime = 1486184400000,
        timeline = null,
        itemDataSet = null;
    
    document.addEventListener("DOMContentLoaded", function(){
        initTimeline();
        getData( displayData );
        
        var upload = document.getElementById("upload");
        upload.onclick = onUpload;
        
    });
    
    function onUpload(){
        var pauses = itemDataSet.get().filter(function(item){ return item.group === "p" });
        var gameplay = itemDataSet.get().filter(function(item){ return item.group !== "p" });

        var newItems = [];

        for (var item of gameplay){
            var idParts = item.id.split('-');

            newItems[idParts[0]] = {
                time: (item.start instanceof Date ? item.start.getTime() : item.start) - startTime,
                key: item.group,
                press: 1
            };

            newItems[idParts[1]] = {
                time: (item.end instanceof Date ? item.end.getTime() : item.end) - startTime,
                key: item.group,
                press: 0
            };

        }

        // add in our pause
        if (pauses.length > 0){
            var time = pauses[0].start.getTime() - startTime;
            for (var itemIndex in newItems){
                if ( newItems[itemIndex].time > time ){
                    newItems.splice(itemIndex, 0, {
                        key: "p",
                        press: "1",
                        time: time
                    });
                    break;
                }
            }
        }

        console.log("our new list", newItems);
        sendData( newItems );
    }
    
    function displayData(items){
        var id = 0;
        itemDataSet = new vis.DataSet();
        
        window._items = itemDataSet;
        var openItems = {
            j: null,
            l: null,
            r: null
        }
        
        for (var itemIndex in items){
            var item = items[itemIndex];
            
            if ( item.press === 1 ){
                openItems[ item.key ] = { 
                    id: itemIndex, 
                    group: item.key, 
                    start: startTime + item.time
                };
            } else {
                openItems[ item.key ].end = startTime + item.time;
                openItems[ item.key ].id += "-" + itemIndex;
                openItems[ item.key ].content = (openItems[ item.key ].end - openItems[ item.key ].start) + " ms";
                itemDataSet.add( openItems[item.key] );
            }
        }
        
        timeline.setItems( itemDataSet );
    }
    
    function getData(cb){
        var oReq = new XMLHttpRequest();
        oReq.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
                rawItems = JSON.parse(this.responseText);
                cb(rawItems);
            }
        }
        oReq.open("GET", "/data");
        oReq.send();
    }
    
    function sendData(data){
        console.log('data', data);
        var oReq = new XMLHttpRequest();
        oReq.open("POST", "/data");
        oReq.send(JSON.stringify(data));
    }
    
    function initTimeline(){
    
        var container = document.getElementById('visualization');

        // create a data set with groups
          var groups = new vis.DataSet();
            groups.add({id: 'p', content: "pause"});
            groups.add({id: 'l', content: "left"});
            groups.add({id: 'r', content: "right"});
            groups.add({id: 'j', content: "jump"});

          var options = { 
              editable: true, 
              showCurrentTime: false,
              min: 1486184400000,
              align: 'center',
              format: {
                  minorLabels: {
                    millisecond:'SSS',
                    second:     's',
                    minute:     'mm',
                    hour:       'HH:mm',
                    weekday:    'ddd D',
                    day:        'D',
                    month:      'MMM',
                    year:       'YYYY'
                  },
                  majorLabels: {
                    millisecond:'mm:ss',
                    second:     'mm',
                    minute:     'ddd D MMMM',
                    hour:       'ddd D MMMM',
                    weekday:    'MMMM YYYY',
                    day:        'MMMM YYYY',
                    month:      'YYYY',
                    year:       ''
                  }
              },
              onMove: function(item, cb){
                  item.content = (item.end.getTime () - item.start.getTime()) + " ms";
                  cb(item);
              }
          };

          timeline = new vis.Timeline(container);
          timeline.setOptions(options);
          timeline.setGroups(groups);
        
        window._time = timeline;

    }
    
})()

