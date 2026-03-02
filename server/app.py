import os, json, subprocess, tempfile, glob
from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS

app = Flask(__name__, static_folder='../frontend/build', static_url_path='')
CORS(app)

# Configuration
PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BINARY_PATH = os.path.join(PROJECT_ROOT, 'genomesift')  # or genomesift.exe on Windows
if os.path.exists(BINARY_PATH + '.exe'):
    BINARY_PATH += '.exe'
    
DATA_DIR = os.path.join(PROJECT_ROOT, 'data')
REPORTS_DIR = os.path.join(PROJECT_ROOT, 'reports')
UPLOAD_DIR = os.path.join(PROJECT_ROOT, 'uploads')

# Create dirs
os.makedirs(REPORTS_DIR, exist_ok=True)
os.makedirs(UPLOAD_DIR, exist_ok=True)


@app.route('/api/samples', methods=['GET'])
def get_samples():
    try:
        def parse_fasta_headers(directory):
            samples = []
            for path in glob.glob(os.path.join(directory, '*.fasta')):
                filename = os.path.basename(path)
                name = filename
                desc = ""
                with open(path, 'r') as f:
                    first_line = f.readline().strip()
                    if first_line.startswith('>'):
                        parts = first_line[1:].split(' ', 1)
                        name = parts[0]
                        if len(parts) > 1:
                            desc = parts[1]
                
                # Make path relative to project root or use absolute path
                rel_path = os.path.relpath(path, PROJECT_ROOT)
                # Ensure forward slashes for cross-platform compatibility
                rel_path = rel_path.replace('\\', '/')
                samples.append({
                    "name": name,
                    "filename": filename,
                    "path": rel_path,
                    "description": desc
                })
            return samples
            
        return jsonify({
            "references": parse_fasta_headers(os.path.join(DATA_DIR, 'reference_sequences')),
            "patients": parse_fasta_headers(os.path.join(DATA_DIR, 'patient_samples'))
        })
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500


@app.route('/api/upload', methods=['POST'])
def upload_file():
    try:
        if 'file' not in request.files:
            return jsonify({"status": "error", "message": "No file part"}), 400
        file = request.files['file']
        if file.filename == '':
            return jsonify({"status": "error", "message": "No selected file"}), 400
            
        filename = file.filename
        file_path = os.path.join(UPLOAD_DIR, filename)
        file.save(file_path)
        
        # Parse header
        name = filename
        desc = ""
        with open(file_path, 'r') as f:
            first_line = f.readline().strip()
            if first_line.startswith('>'):
                parts = first_line[1:].split(' ', 1)
                name = parts[0]
                if len(parts) > 1:
                    desc = parts[1]
            else:
                os.remove(file_path)
                return jsonify({"status": "error", "message": "Invalid FASTA format: must start with >"}), 400
                
        rel_path = os.path.relpath(file_path, PROJECT_ROOT).replace('\\', '/')
        return jsonify({"path": rel_path, "name": name, "description": desc})
        
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500


@app.route('/api/align', methods=['POST'])
def align():
    try:
        data = request.json
        ref_path = data.get('reference_path')
        patient_path = data.get('patient_path')
        algo = data.get('algorithm', 'global')
        gene = data.get('gene', 'HBB')
        match = data.get('match', 1)
        mismatch = data.get('mismatch', -1)
        gap = data.get('gap', -2)
        
        if not ref_path or not patient_path:
            return jsonify({"status": "error", "message": "Missing reference or patient path"}), 400
            
        cmd = [
            BINARY_PATH, '--json',
            '--ref', ref_path,
            '--patient', patient_path,
            '--algorithm', algo,
            '--gene', gene,
            '--match', str(match),
            '--mismatch', str(mismatch),
            '--gap', str(gap)
        ]
        
        print("Running CMD:", ' '.join(cmd))
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=30, cwd=PROJECT_ROOT)
        
        if result.returncode != 0:
            print("STDERR:", result.stderr)
            # Find the JSON in stdout if it exists despite nonzero return
            try:
                out_json = json.loads(result.stdout.strip())
                return jsonify(out_json), 400
            except:
                return jsonify({
                    "status": "error", 
                    "message": f"Engine failed with code {result.returncode}",
                    "details": result.stderr or result.stdout
                }), 500
                
        try:
            # We must find the JSON block. Strip anything before the first '{'
            output = result.stdout.strip()
            bracket_idx = output.find('{')
            if bracket_idx != -1:
                output = output[bracket_idx:]
            
            out_json = json.loads(output)
            return jsonify(out_json)
        except json.JSONDecodeError as e:
            print("Failed to decode JSON. Raw stdout:", result.stdout)
            return jsonify({"status": "error", "message": "Failed to parse engine output as JSON"}), 500
            
    except subprocess.TimeoutExpired:
        return jsonify({"status": "error", "message": "Alignment timed out (> 30s)"}), 504
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500


@app.route('/api/report', methods=['POST'])
def generate_report():
    try:
        data = request.json
        # Quick fallback if the client explicitly requests a report creation 
        # (Though C++ generator could do it, the python can do it given JSON)
        # We will format it elegantly here.
        import string
        import random
        report_id = ''.join(random.choices(string.ascii_lowercase + string.digits, k=6))
        filename = f"report_{report_id}.txt"
        file_path = os.path.join(REPORTS_DIR, filename)
        
        align = data.get('alignment', {})
        muts = data.get('mutations', [])
        summary = data.get('summary', {})
        
        with open(file_path, 'w') as f:
            f.write("========== GENOMESIFT ANALYSIS REPORT ==========\n\n")
            f.write(f"Reference: {align.get('seq1_name', 'Unknown')}\n")
            f.write(f"Patient:   {align.get('seq2_name', 'Unknown')}\n")
            f.write(f"Algorithm: {align.get('algorithm', 'Unknown')}\n\n")
            
            f.write("--- ALIGNMENT STATS ---\n")
            f.write(f"Score:      {align.get('score', 0)}\n")
            f.write(f"Identity:   {align.get('identity_percentage', 0):.2f}%\n")
            f.write(f"Matches:    {align.get('matches', 0)}\n")
            f.write(f"Mismatches: {align.get('mismatches', 0)}\n")
            f.write(f"Gaps:       {align.get('gaps', 0)}\n\n")
            
            f.write(f"--- DETECTED MUTATIONS ({len(muts)}) ---\n")
            for m in muts:
                f.write(f"Position {m.get('position')}: {m.get('reference')} -> {m.get('variant')} [{m.get('type')}]\n")
                f.write(f"  > Disease:  {m.get('disease')}\n")
                f.write(f"  > Severity: {m.get('severity')}\n")
                f.write(f"  > Gene:     {m.get('gene')}\n\n")
                
            f.write("--- TRANSCRIPTION ---\n")
            f.write(f"RNA:     {data.get('transcription', {}).get('rna', '')}\n")
            f.write(f"Protein: {data.get('transcription', {}).get('protein', '')}\n")
            
        # Give relative path for download
        rel_path = f"reports/{filename}"
        return jsonify({
            "path": rel_path, 
            "download_url": f"/api/download/{filename}",
            "filename": filename
        })
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500


@app.route('/api/download/<filename>', methods=['GET'])
def download_file(filename):
    return send_from_directory(REPORTS_DIR, filename, as_attachment=True)


@app.route('/api/disease-db', methods=['GET'])
def get_disease_db():
    try:
        entries = []
        db_path = os.path.join(DATA_DIR, 'disease_db.csv')
        if os.path.exists(db_path):
            with open(db_path, 'r') as f:
                lines = f.readlines()
                if not lines:
                    return jsonify({"entries": []})
                headers = [h.strip() for h in lines[0].split(',')]
                for line in lines[1:]:
                    if not line.strip(): continue
                    parts = [p.strip() for p in line.split(',')]
                    entry = dict(zip(headers, parts))
                    # Parse position as int for frontend filtering
                    if 'position' in entry:
                        try: entry['position'] = int(entry['position'])
                        except: pass
                    entries.append(entry)
        return jsonify({"entries": entries})
    except Exception as e:
        return jsonify({"status": "error", "message": str(e)}), 500


@app.route('/', defaults={'path': ''})
@app.route('/<path:path>')
def serve(path):
    if path != "" and os.path.exists(app.static_folder + '/' + path):
        return send_from_directory(app.static_folder, path)
    else: # Provide simple fallback if built react app is missing
        if os.path.exists(os.path.join(app.static_folder, 'index.html')):
            return send_from_directory(app.static_folder, 'index.html')
        return "React Frontend not built yet. Run 'npm run build' in frontend directory.", 404


if __name__ == '__main__':
    if not os.path.exists(BINARY_PATH):
        print(f"[WARNING] GenomeSift binary not found at {BINARY_PATH}! Run 'make' first.")
    else:
        print(f"[INFO] GenomeSift binary found at {BINARY_PATH}")
        
    print("[INFO] Starting GenomeSift API server...")
    print("[INFO] Frontend: http://localhost:5000")
    print("[INFO] API: http://localhost:5000/api/")
    app.run(debug=True, port=5000)
