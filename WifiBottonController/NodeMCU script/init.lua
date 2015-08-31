commands = {}
commands.get1 = "GET http://10.0.0.15/port_3480/data_request?id=lu_action&output_format=json&DeviceNum="
commands.get2 = "&serviceId=urn:upnp-org:serviceId:Dimming1&action=SetLoadLevelTarget&newLoadlevelTarget="
commands.get3 = " HTTP/1.1\r\n"
commands.host = "Host: 10.0.0.15\r\n"
commands.connection = "Connection: keep-alive\r\n"
commands.accept = "Accept: */*\r\n\r\n"

print("Starting wifi environment for button controller")
wifi.setmode(wifi.STATION)
wifi.sta.config("SSID","password")
wifi.sta.connect()

tmr.alarm(1, 1000, 1, function() 
    if wifi.sta.getip()== nil then 
        print("IP unavaiable, Waiting...") 
    else 
        tmr.stop(1)
        print("Config done, IP is "..wifi.sta.getip())
    end 
end)
 
function setLed(deviceNum, level)
    print("setLed("..deviceNum..", "..level..") received")
    conn = net.createConnection(net.TCP, false) 
    conn:on("receive", function(conn, payload)
        --local payload = string.match(payload, "{.*}")
        print(payload)
        print("\r\n")
    end )
    conn:connect(80,"10.0.0.15")
    command = commands.get1 .. deviceNum .. commands.get2 .. level .. commands.get3 .. commands.host .. commands.connection .. commands.accept
    --print(command)
    conn:send(command)
end