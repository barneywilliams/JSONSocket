require 'rubygems' # Necessary for Ruby 1.8 (1.9 loads rubygems by default)
$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/RubyServer')
require 'rake'
require 'json_server'

desc "Start server and client then wait for exchange completion"
task :default => [:server, :client, :wait]

desc "Start Ruby JSON server"
task :server do
  @server = JSONServer.new
  @server.start
end

desc "Start Qt JSON client"
task :client do
  @client_thread = Thread.new do
    system "./QtClient/JSONClient"
  end
end

desc "Wait for server and client to shutdown"
task :wait do
  puts "Waiting for client to shutdown..."
  @client_thread.join(30)
  puts "Waiting for server to shutdown..."
  @server.wait_for_completion
  puts "DONE!"
end

