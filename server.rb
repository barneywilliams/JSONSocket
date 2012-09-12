require 'socket'

server = TCPServer.new 19024
p "VT server started!"
@vt_server_thread = Thread.start(server.accept) do |client|
  p "VT server connection established!"
  client.puts "Hello !"
  while msg = client.gets
     puts "<: #{msg}"
  end
  client.close
end