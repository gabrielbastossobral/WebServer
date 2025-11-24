const responseDiv = document.getElementById('response');

function displayResponse(status, statusText, headers, body, isImage = false, imagePath = '', isHtml = false) {
    let statusClass = status >= 200 && status < 300 ? 'status-success' : 
                      status >= 400 ? 'status-error' : 'status-info';
    
    let response = `<span class="status ${statusClass}">Status: ${status} ${statusText}</span>\n\n`;
    response += '=== HEADERS ===\n';
    for (let [key, value] of Object.entries(headers)) {
        response += `${key}: ${value}\n`;
    }
    response += '\n=== BODY ===\n';
    
    if (isImage) {
        response += `[IMAGE FILE - ${body.length} bytes]\n\n`;
        response += `<img src="${imagePath}" style="max-width: 100%; border: 2px solid #ddd; border-radius: 5px; margin-top: 10px;" alt="Loaded image">`;
    } else if (isHtml) {
        response += '[HTML CONTENT - Rendered below]\n\n';
        response += '<div style="border: 2px solid #667eea; border-radius: 5px; padding: 20px; background: #f9f9f9; margin-top: 10px;">';
        response += body;
        response += '</div>';
    } else {
        let escapedBody = body
            .replace(/&/g, '&amp;')
            .replace(/</g, '&lt;')
            .replace(/>/g, '&gt;')
            .replace(/"/g, '&quot;')
            .replace(/'/g, '&#039;');
        
        response += escapedBody || '(empty)';
    }
    
    responseDiv.innerHTML = response;
}

function displayError(error) {
    responseDiv.innerHTML = `<span class="status status-error">ERROR</span>\n\n${error}`;
}

function isValidPath(path) {
    if (path.match(/^[a-zA-Z]+:\/\//)) {
        return false;
    }
    if (!path.startsWith('/')) {
        return false;
    }
    return true;
}

function isImagePath(path) {
    const imageExtensions = ['.jpg', '.jpeg', '.png', '.gif', '.bmp', '.webp', '.svg', '.ico'];
    const lowerPath = path.toLowerCase();
    return imageExtensions.some(ext => lowerPath.endsWith(ext));
}

async function testGet(path) {
    try {
        const response = await fetch(path);
        const headers = {};
        response.headers.forEach((value, key) => headers[key] = value);
        
        const contentType = headers['content-type'] || '';
        const isImage = contentType.startsWith('image/') || isImagePath(path);
        const isHtml = contentType.includes('text/html');
        
        if (isImage) {
            const blob = await response.blob();
            const body = `Binary data (${blob.size} bytes)`;
            displayResponse(response.status, response.statusText, headers, body, true, path, false);
        } else {
            const body = await response.text();
            displayResponse(response.status, response.statusText, headers, body, false, '', isHtml);
        }
    } catch (error) {
        displayError(error.message);
    }
}

function testCustomGet() {
    let path = document.getElementById('customGetPath').value || '/';
    
    if (!isValidPath(path)) {
        displayError('Invalid path! Please use relative paths starting with "/" (e.g., /test.html, /images/photo.jpg)\n\nDo NOT use full URLs like http://example.com');
        return;
    }
    
    testGet(path);
}

async function testPost() {
    let filename = document.getElementById('postFilename').value.trim();
    
    if (!filename) {
        displayError('Please enter a filename!');
        return;
    }
    
    if (!filename.includes('.')) {
        filename += '.json';
        document.getElementById('postFilename').value = filename;
    }
    
    const path = '/upload/' + filename;
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
    let filename = document.getElementById('postFormFilename').value.trim();
    
    if (!filename) {
        displayError('Please enter a filename!');
        return;
    }
    
    if (!filename.includes('.')) {
        filename += '.txt';
        document.getElementById('postFormFilename').value = filename;
    }
    
    const path = '/upload/' + filename;
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
    const filename = document.getElementById('deleteFilename').value.trim();
    
    if (!filename) {
        displayError('Please enter a filename!');
        return;
    }
    
    const cleanFilename = filename.startsWith('/') ? filename.substring(1) : filename;
    const path = '/upload/' + cleanFilename;
    
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
        const response = await fetch('/upload/' + file.name, {
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
    if (path && isValidPath(path)) {
        testCGI(path);
    } else if (path) {
        displayError('Invalid CGI path! Please use relative paths starting with "/"');
    }
}

responseDiv.innerHTML = '👋 Ready to test your WebServ!\n\nClick any button to start testing HTTP methods.\n\n💡 All POST/DELETE operations work in /upload/ folder';