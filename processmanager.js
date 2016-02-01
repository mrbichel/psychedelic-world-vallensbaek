var express = require('express');
var serveIndex = require('serve-index');
var child_process = require('child_process');
var exec = require('child_process').exec;
var fs = require('fs');
var path = require('path');
var rimraf = require('rimraf');
var Logger = require('le_node');
var winston = require('winston');
var swig = require('swig');
var nconf = require('nconf');
var os = require('os');

function ethanProcessManager() {

var TYPE = "johan"; // "ole"

// TODO set logentries token dynamically for host

var childProcesses = [];
var respawnThreshold;

function ChildProcess(exe, cwd) {
  this.child = null;
  this.exe = exe;
  this.cwd = cwd;

  this.isRunning = false;
  this.isStoppedManually = false;
  this.lastSpawnTime = 0;
  this.immediateErrorRespawns = 0; // stop attempting to respawn if killed immediate a number of times in a row 
}

function fL(message) {
  var s = "hostname:" + os.hostname() + " " + message;
  return s;
}

// see: https://github.com/logentries/le_node/issues/87 - reverted winston for now 
var logger = new (winston.Logger)({
    transports: [
      new (winston.transports.Console) ({ level: "info", 
        colorize: true,
      }),

      new winston.transports.DailyRotateFile({
        name: 'file',
        level: "info",
        datePattern: '.yyyy-MM-dd',
        filename: path.join(__dirname + "/../log/", "ethan-"+TYPE+".log")
      })
      /*new (winston.transports.Logentries) ({
         token: logentriesToken,
         level: "info",
          })
    ]*/
});

var app = express();
app.engine('html', swig.renderFile);
app.set('view engine', 'html');
app.set('views', __dirname + '/views');
app.use(express.static(__dirname + "/static/"));

app.get('/api', function(req, res) {

  var found = false;
  var processName = req.query.process;
  childProcesses.forEach(function (process) {
      if(processName == process.exe) {
        if(req.query.cmd == "stop") {
          process.isStoppedManually = true;
          process.child.kill('SIGINT');
          res.setHeader('Content-Type', 'text/plain');
          res.send(process.exe + ' has been stopped');
          found = true;
          
        } else if(req.query.cmd == "start") {
          process.immediateErrorRespawns = 0;
          spawnProcess(process);
          res.setHeader('Content-Type', 'text/plain');
          res.send(process.exe + ' has been started');
          found = true;
      }
    }
  });

  if(!found) {
    res.send('process api: process or command not found'); 
  }
  
});

app.get('/api/getProcessState', function(req, res) {
    res.setHeader('Content-Type', 'application/json');

    var fProcesses = [];

    childProcesses.forEach(function (process) {

      fProcesses.push({
          exe: process.exe,
          lastSpawnTime: process.lastSpawnTime,
          isRunning: process.isRunning, 
          isStoppedManually: process.isStoppedManually })
    });

    var resp = {type: TYPE, processes: fProcesses};
    res.send(JSON.stringify(resp));

});

app.get('/dashboard', function(req, res) {
  res.render('dashboard-'+TYPE, { type: TYPE, processes: childProcesses });
});

var spawnProcess = function(childObject) {
  if(childObject.isRunning) return;

	logger.info(fL("Spawning " + childObject.exe));
	childObject.child = child_process.spawn(childObject.exe, {
	   cwd: childObject.cwd,
  });

	// Listen for an exit event:
   childObject.child.on('exit', function (exitCode) {
    childObject.isRunning = false;

    if(childObject.isStoppedManually) {
      logger.info(fL(childObject.exe + " was stopped manually."));
    } else {

      logger.error(fL(childObject.exe + " exited with code: " + exitCode + "."));

      if(Date.now() - childObject.lastSpawnTime < 30000 && exitCode != 0) {

        if(childObject.immediateErrorRespawns < 30) {
            logger.info(fL(childObject.exe + " will respawn in 10 seconds."));
            setTimeout(function() {
                spawnProcess(childObject);
                }, 10000 );
            childObject.immediateErrorRespawns += 1;
        } else {
            logger.error(fL(childObject.exe + " has exited immediately more than 30 times consecutively. Respawns disabled pending manual restart."));
        }
      } else {
        logger.error(fL(childObject.exe + "  will respawn in 2 seconds."));
        setTimeout(function() {
                spawnProcess(childObject);
                }, 2000 );
        childObject.immediateErrorRespawns = 0;
      }
    }
   });

    // Redirect logs from child
   childObject.child.stdout.on('data', function (data) {
    logger.info(fL("[" + childObject.exe + "] " + data));
   });

    // Redirect logs from child
   childObject.child.stderr.on('data', function (data) {
    logger.error(fL("[" + childObject.exe + "] " + data));
   });

   childObject.lastSpawnTime = Date.now();
   childObject.isRunning = true;
   childObject.isStoppedManually = false;
}

/// johan
if(TYPE == "johan") {
  
  var mainProcess = new ChildProcess("vallensbak.app", __dirname + "/../bin/vallensbak");
  childProcesses.push(mainProcess);
  spawnProcess(mainProcess);

  // start madmapper 


// ole
} else if(TYPE == "ole") {
  
  var mainProcess = new ChildProcess("multiplicity.app", __dirname + "/../bin/multiplicity/");
  childProcesses.push(mainProcess);
  spawnProcess(mainProcess);


}

// start express server
logger.info(fL("[HTTP] Starting http server listening on 127.0.0.1:8080"));
app.listen(8080);

}

ethanProcessManager();