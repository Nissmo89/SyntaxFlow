import os
import sys
import urllib.request
import tarfile
import zipfile
import subprocess
import shutil
import platform

def get_wasmer_url():
    system = platform.system().lower()
    machine = platform.machine().lower()
    
    base_url = "https://github.com/wasmerio/wasmer/releases/latest/download/"
    
    if system == "linux":
        if "aarch64" in machine or "arm64" in machine:
            return base_url + "wasmer-linux-aarch64.tar.gz"
        else:
            return base_url + "wasmer-linux-amd64.tar.gz"
    elif system == "darwin":
        if "arm64" in machine or "aarch64" in machine:
            return base_url + "wasmer-darwin-aarch64.tar.gz"
        else:
            return base_url + "wasmer-darwin-amd64.tar.gz"
    elif system == "windows":
        return base_url + "wasmer-windows-amd64.tar.gz"
    else:
        print(f"Unsupported system: {system} {machine}")
        sys.exit(1)

def main():
    root_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
    tools_dir = os.path.join(root_dir, "tools")
    os.makedirs(tools_dir, exist_ok=True)
    
    wasmer_dir = os.path.join(tools_dir, "wasmer")
    
    if platform.system().lower() == "windows":
        wasmer_exe = os.path.join(wasmer_dir, "bin", "wasmer.exe")
    else:
        wasmer_exe = os.path.join(wasmer_dir, "bin", "wasmer")
        
    if os.path.exists(wasmer_exe):
        print(f"Wasmer is already installed at {wasmer_exe}")
    else:
        os.makedirs(wasmer_dir, exist_ok=True)
        url = get_wasmer_url()
        print(f"Downloading Wasmer from {url}...")
        archive_path = os.path.join(tools_dir, "wasmer_archive")
        req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
        with urllib.request.urlopen(req) as response, open(archive_path, 'wb') as out_file:
            shutil.copyfileobj(response, out_file)
        
        print("Extracting Wasmer...")
        if url.endswith(".zip"):
            with zipfile.ZipFile(archive_path, 'r') as zip_ref:
                zip_ref.extractall(wasmer_dir)
        else:
            with tarfile.open(archive_path, "r:gz") as tar_ref:
                tar_ref.extractall(wasmer_dir)
                
        os.remove(archive_path)
        
        if platform.system().lower() != "windows":
            os.chmod(wasmer_exe, 0o755)
            
        print(f"Wasmer installed to {wasmer_exe}")

    print("Verifying Wasmer installation...")
    try:
        subprocess.run([wasmer_exe, "--version"], check=True)
        print("Wasmer verified successfully!")
    except subprocess.CalledProcessError as e:
        print(f"Failed to verify Wasmer: {e}")
        sys.exit(1)
        
if __name__ == "__main__":
    main()
