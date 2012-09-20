require 'socket'
require 'json'

class JSONServer

   def initialize
      @server = TCPServer.new 19024
   end

   def start
      @server_thread = Thread.new do
         @client_thread = Thread.start(@server.accept) do |client|
            puts "server: Accepted client connection!"
            send_request(client, 'greeting', :message => 'Hello server!')
            receive_response(client)
            send_request(client, 'shutdown')
            receive_response(client)
            client.close
         end
      end
   end

   def wait_for_completion
      sleep (0.5) while !@client_thread
      @client_thread.join
      @server_thread.join
   end

   def send_request(client, command, args={})
      data = {'request' => {'command' => command}}
      data['request'].merge!(args)
      request = JSON.generate(data)
      client.print request
      puts "server: >> #{request}"
   end

   def receive_response(client)
      response = client.recv(1024)
      puts "server: << #{response}"
      data = JSON.parse(response)
      return data
   end

end
