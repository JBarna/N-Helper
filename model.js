const DELIM = ":";
exports.record = [];

exports.parse = function(input){
    exports.record = [];
    
    var parts = input.split(':');
    var i = 2;
    
    while( parts[i] !== "END"){
        
        var newRec = {
            key: parts[i++],
            press: +parts[i++],
            time: +parts[i++]
        }
        
        exports.record.push(newRec);
    }
    
    console.log(exports.record);
    
}

exports.stringify = function(newRecord){
    exports.record = newRecord;
    var ret = "START:" + newRecord.length + DELIM;
    
    for (var record of newRecord)
        ret += record.key + DELIM + record.press + DELIM + record.time + DELIM;
    
    ret += "END:";    
    return ret;
}