const spawn = require('child_process').spawn,
    path = require('path'),
    fs = require('fs'),
    model = require('./model.js');

const exePath = path.join(__dirname,  "n++helper.exe");

var exe;

exports.init = function(serialPort){
    // check to see if we have the executable 
    if (!fs.existsSync( exePath )){
        console.error("Cannot find executable to record keystrokes");
        process.exit(1);
    }
    
    // start the process
    exe = spawn(exePath);
    
    exe.stdout.pipe(serialPort);
    
    var _in = "";
    exe.stdout.on('data', chunk => {
        _in += chunk;
        if (_in.indexOf("END:") >= 0){
            model.parse(_in);
            _in = "";
        }
    });
    
    exe.on('close', () => console.log("ITS CLOSED FOLKS"));
    
    console.log(`c++ program running at pid ${exe.pid}`);
    
}