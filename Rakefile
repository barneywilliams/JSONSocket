require 'rubygems' # Necessary for Ruby 1.8 (1.9 loads rubygems by default)
$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/RubyServer')
require 'rake'
require 'json_server'

desc "Start server and client then wait for exchange completion"
task :run => [:server, :client, :wait]

desc "Start server and client then wait for exchange completion"
task :default => [:build, :run]

desc "Start Ruby JSON server"
task :server do
  @server = JSONServer.new
  @server.start
end

desc "Build Qt JSON client"
task :build do
  FileUtils.cd("QtClient") do
    sh "make"
  end
end

desc "Start Qt JSON client"
task :client do
  @client_thread = Thread.new do
    system "./QtClient/JSONClient"
  end
end

desc "Wait for server and client to shutdown"
task :wait do
  @client_thread.join
  @server.wait_for_completion
end
