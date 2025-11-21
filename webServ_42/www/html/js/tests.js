const responseDiv = document.getElementById('response');

        function displayResponse(status, statusText, headers, body) {
            let statusClass = status >= 200 && status < 300 ? 'status-success' : 
                              status >= 400 ? 'status-error' : 'status-info';
            
            let response = `<span class="status ${statusClass}">Status: ${status} ${statusText}</span>\n\n`;
            response += '=== HEADERS ===\n';
            for (let [key, value] of Object.entries(headers)) {
                response += `${key}: ${value}\n`;
            }
            response += '\n=== BODY ===\n';
            response += body || '(empty)';
            
            responseDiv.innerHTML = response;
        }

        function displayError(error) {
            responseDiv.innerHTML = `<span class="status status-error">ERROR</span>\n\n${error}`;
        }

        async function testGet(path) {
            try {
                const response = await fetch(path);
                const body = await response.text();
                const headers = {};
                response.headers.forEach((value, key) => headers[key] = value);
                displayResponse(response.status, response.statusText, headers, body);
            } catch (error) {
                displayError(error.message);
            }
        }

        function testCustomGet() {
            const path = document.getElementById('customGetPath').value || '/';
            testGet(path);
        }

        async function testPost() {
            const path = document.getElementById('postPath').value;
            const body = document.getElementById('postBody').value;
            
            try {
                const response = await fetch(path, {
                    method: 'POST',
                    headers: {
                        'Content-Type': 'application/json'
                    },
                    body: body
                });
                const responseBody = await response.text();
                const headers = {};
                response.headers.forEach((value, key) => headers[key] = value);
                displayResponse(response.status, response.statusText, headers, responseBody);
            } catch (error) {
                displayError(error.message);
            }
        }

        async function testPostForm() {
            const path = document.getElementById('postPath').value;
            const formData = new FormData();
            formData.append('field1', 'value1');
            formData.append('field2', 'value2');
            
            try {
                const response = await fetch(path, {
                    method: 'POST',
                    body: formData
                });
                const responseBody = await response.text();
                const headers = {};
                response.headers.forEach((value, key) => headers[key] = value);
                displayResponse(response.status, response.statusText, headers, responseBody);
            } catch (error) {
                displayError(error.message);
            }
        }

        async function testDelete() {
            const path = document.getElementById('deletePath').value;
            
            try {
                const response = await fetch(path, {
                    method: 'DELETE'
                });
                const body = await response.text();
                const headers = {};
                response.headers.forEach((value, key) => headers[key] = value);
                displayResponse(response.status, response.statusText, headers, body);
            } catch (error) {
                displayError(error.message);
            }
        }

        async function testFileUpload() {
            const fileInput = document.getElementById('fileUpload');
            const file = fileInput.files[0];
            
            if (!file) {
                displayError('Please select a file first!');
                return;
            }
            
            const formData = new FormData();
            formData.append('file', file);
            
            try {
                const response = await fetch('/upload', {
                    method: 'POST',
                    body: formData
                });
                const body = await response.text();
                const headers = {};
                response.headers.forEach((value, key) => headers[key] = value);
                displayResponse(response.status, response.statusText, headers, body);
            } catch (error) {
                displayError(error.message);
            }
        }

        async function testCGI(path) {
            testGet(path);
        }

        function testCustomCGI() {
            const path = document.getElementById('cgiPath').value;
            if (path) testCGI(path);
        }

        // Initial message
        responseDiv.innerHTML = '👋 Ready to test your WebServ!\n\nClick any button to start testing HTTP methods.';