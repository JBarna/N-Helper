const RecordKeys = require('./RecordKeys'),
      Serial = require('./Serial'),
      Server = require('./server');

Serial.init(RecordKeys.init);
Server.init();