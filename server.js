const express = require('express'),
      app = express(),
      path = require('path'),
      model = require('./model'),
      Serial = require('./Serial');

exports.init = function(){
    // serve static files from public directory
    app.use(express.static('public'));

    // routes
    app.get('/data', (req, res) => {
        res.json( model.record || {} );
    });
    
    app.post('/data', (req, res) => {
        
        var data = "";
        req.on('data', chunk => data += chunk);
        req.on('end', () => {
            
            data = JSON.parse(data);
            console.log("received data", data);
            Serial.send(model.stringify(data));
            
            res.sendStatus(200);
            res.end();
        })
        
    })

    // serve
    app.listen(3000, function () {
      console.log('Example app listening on port 3000!')
    })
}

