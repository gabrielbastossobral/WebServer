#!/usr/bin/env python3

import time
import os

start = time.time()

# Simular processamento
iterations = 1000000
result = sum(i * i for i in range(iterations))

elapsed = time.time() - start

while(1)
	print(12)

print("Content-Type: text/html")
print()

print(f"""
<html>
<head>
    <title>CGI Performance Test</title>
</head>
<body>
    <h1>⚡ CGI Performance Test</h1>
    <div class="metric">
        <strong>Iterations:</strong> {iterations:,}<br>
        <strong>Result:</strong> {result:,}<br>
        <strong>Time:</strong> {elapsed:.4f} seconds<br>
        <strong>Speed:</strong> {iterations/elapsed:,.0f} ops/sec
    </div>
    <div class="metric">
        <strong>PID:</strong> {os.getpid()}<br>
        <strong>Working Dir:</strong> {os.getcwd()}
    </div>
</body>
</html>
""")