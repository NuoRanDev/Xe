import os

def build_rust():
    os.system("cargo build --target-dir \"./../SDK/\"")

build_rust()