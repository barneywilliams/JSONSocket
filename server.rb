require 'socket'

server = TCPServer.new 19024
puts "VT server started!"
@vt_server_thread = Thread.start(server.accept) do |client|
  puts "VT client connection established!"
  client.print "{'test': 'Hello!'}"
  response = client.gets
  puts "<: #{response}"
  client.close
end

@vt_server_thread.join
puts "VT server terminated!"
