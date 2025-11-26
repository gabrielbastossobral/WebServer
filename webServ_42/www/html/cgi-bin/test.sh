#!/bin/bash
# filepath: /home/gbriel/WebServer/webServ_42/www/html/cgi-bin/test.sh

echo "Content-Type: text/html"
echo ""
echo "<html>"
echo "<head><title>CGI Test - Shell</title></head>"
echo "<body>"
echo "<h1>🚀 CGI Shell Script Works!</h1>"
echo "<p>This is a test CGI script written in Bash.</p>"
echo "<p>Current date: $(date)</p>"
echo "<p>Server time: $(date '+%Y-%m-%d %H:%M:%S')</p>"
echo "<p>Working directory: $(pwd)</p>"
echo "</body>"
echo "</html>"