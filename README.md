JSONSocket - A Simple Command Processor Using Ruby and Qt
=========================================================

This is a simple implementation of a command processor with a Ruby server and Qt client. It uses standard JSON for communication, and utilizes the Qt QScriptEngine for parsing received JSON.

Since Qt doesn't have native support for easily serializing data structures to JSON, the JSON is generated in a somewhat manual fashion. Although, generating JSON is fairly trivial. Parsing JSON in Qt is fairly trivial with QScriptEngine, and also has nice mechansims for validating and digging into JSON elements.

Though there are other C++ libraries for JSON, this example uses stock Qt libraries only for simplicity and demonstration of the raw capabilities for JSON exchanges.

Building and Running the Example
================================

The repo includes a Ruby Rakefile to facilitate building and running the example. You can execute 'rake -T' in order to get a listing of all the described tasks.

Although, in order to build the QtClient command-line application and run the example in one shot, you can simply call Rake with no arguments:

	> rake
