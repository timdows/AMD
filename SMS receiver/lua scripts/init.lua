print("Starting wifi environment for sim900 module")
wifi.setmode(wifi.STATION)
wifi.sta.config("SSID","PASSWORD")
wifi.sta.connect()

tmr.alarm(1, 1000, 1, function() 
    if wifi.sta.getip()== nil then 
        print("IP unavaiable, Waiting...") 
    else 
        tmr.stop(1)
        print("Config done, IP is "..wifi.sta.getip())
        startServer()
    end 
 end)

function startServer()
  srv=net.createServer(net.TCP,180) 
  srv:listen(2323,function(conn) 
    conn:send("Welcome to the SIM900 environment")
  
    -- UART receive, TCP send
    uart.on("data", 0, function(data)
      conn:send(data)
    end, 0)
    
    -- TCP receive, UART send
    conn:on("receive", function(conn, data)
      uart.write(0, data)
    end)

    conn:on("disconnection",function(conn) 
      print("disconnected") 
    end) 
  end)  
end