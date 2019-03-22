const https = require('https');

const options = {
  hostname: 'api.github.com',
  path: '/emojis',
  // The GitHub API requires a User-Agent header
  headers: { 'User-Agent': 'MyUserAgent/0.1' }
};

https.get(options, (resp) => {
  let data = '';

  resp.on('data', (chunk) => {
    data += chunk;
  });

  resp.on('end', () => {
    console.log(JSON.parse(data));
  });

}).on("error", (err) => {
  console.log("Error: " + err.message);
});

