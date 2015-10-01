print("Starting wifi environment for Patrick")
wifi.setmode(wifi.STATION)
wifi.sta.config("SSID","PASS")
wifi.sta.connect()

tmr.alarm(1, 1000, 1, function() 
    if wifi.sta.getip()== nil then 
        print("IP unavaiable, Waiting...") 
    else 
        tmr.stop(1)
        print("Config done, IP is "..wifi.sta.getip())
    end 
 end)

 sendFileContents = function(conn, filename) 
    if file.open(filename, "r") then 
        --conn:send(responseHeader("200 OK","text/html")); 
        repeat  
        local line=file.readline()  
        if line then  
            conn:send(line); 
        end  
        until not line  
        file.close(); 
    else 
        conn:send(responseHeader("404 Not Found","text/html")); 
        conn:send("Page not found"); 
            end 
end 

responseHeader = function(code, type) 
    return "HTTP/1.1 " .. code .. "\r\nConnection: close\r\nServer: nunu-Luaweb\r\nContent-Type: " .. type .. "\r\n\r\n";  
end 

httpserver = function () 
 srv=net.createServer(net.TCP)  
    srv:listen(80,function(conn)  
      conn:on("receive",function(conn,request)  
        conn:send(responseHeader("200 OK","text/html")); 
        local buf = "";
        local _, _, method, path, vars = string.find(request, "([A-Z]+) (.+)?(.+) HTTP");
        if(method == nil)then 
            _, _, method, path = string.find(request, "([A-Z]+) (.+) HTTP"); 
        end
        
        if string.find(request,"state=") then 
          print("STATE" .. string.gsub(path, "/state=", ""));
        elseif string.find(request,"led=") then 
          print("LED" .. string.gsub(path, "/led=", ""));
        else 
            sendFileContents(conn,"header.htm"); 
        end 
        --debug
        --print(request); 
      end)  
      conn:on("sent",function(conn)  
        conn:close();  
        conn = nil;     
 
      end) 
    end) 
end 
 
httpserver()