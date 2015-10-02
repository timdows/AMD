commands = {}
commands.get1 = "GET http://10.0.0.14/housedb/web/app_dev.php/vera/7segment"
commands.get2 = " HTTP/1.1\r\n"
commands.host = "Host: 10.0.0.14\r\n"
commands.connection = "Connection: keep-alive\r\n"
commands.accept = "Accept: */*\r\n\r\n"

print("Starting wifi environment for json vera getter")
wifi.setmode(wifi.STATION)
wifi.sta.config("ssid","password")
wifi.sta.connect()

tmr.alarm(1, 1000, 1, function() 
    if wifi.sta.getip()== nil then 
        print("IP unavaiable, Waiting...") 
    else 
        tmr.stop(1)
        print("Config done, IP is "..wifi.sta.getip())
    end 
end)
 
function getData()
    conn = net.createConnection(net.TCP, false) 
    conn:on("receive", function(conn, payload)
        local payload = string.match(payload, "{.*}")
        print(payload)
        print("\r\n")
    end )
    conn:connect(80,"10.0.0.14")
    command = commands.get1 .. commands.get2 .. commands.host .. commands.connection .. commands.accept
    --print(command)
    conn:send(command)
end