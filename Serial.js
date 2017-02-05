const SerialPort = require('serialport');

exports.init = function(cb){
    SerialPort.list( (err, ports) => {
        if (err || ports.length === 0){ 
            console.error("error gettings com ports");
            process.exit(1);
        }

        // open first com port
        exports.port = new SerialPort( ports[0].comName, err => {
            if (err == null){
                console.log("Opened port " + ports[0].comName);
                cb(exports.port);
            }
            else {
                console.error("Failed to open port " + ports[0].comName, err);
                process.exit(1);
            }
        });

        exports.port.on('data', chunk => console.log( "chunk: " + chunk.toString()));
    });
}

exports.send = function(data){
    exports.port.write(data);
}
