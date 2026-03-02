# Running GenomeSift with Web UI

## Prerequisites
- C++ compiler (g++ with C++17 support)
- Python 3.8+
- Node.js 16+

## Step 1: Compile C++ Backend
```bash
cd GenomeSift
make clean
make
```

## Step 2: Install Python Dependencies
```bash
pip install flask flask-cors
```

## Step 3: Install Frontend Dependencies
```bash
cd frontend
npm install
```

## Step 4: Build Frontend (for production)
```bash
npm run build
```

## Step 5: Run the Server
```bash
cd ../server
python app.py
```

## Step 6: Open in Browser
Navigate to http://localhost:5000

## Development Mode (hot reload)
```bash
# Terminal 1: Flask backend
cd server && python app.py

# Terminal 2: React dev server
cd frontend && npm start
# Frontend runs on http://localhost:3000, proxies API to :5000
```

## Quick Test
1. Open http://localhost:5000
2. Click "Quick Demo"
3. Watch the alignment results appear automatically

## Troubleshooting
- If "binary not found": run `make` in the project root first
- If CORS errors: make sure `flask-cors` is installed
- If port 5000 in use: change port in `app.py`
- On Windows: the binary is `genomesift.exe`, the server auto-detects this
