{
  description = "Turret_2 — CMake/SDL3/OpenGL/AngelScript game build";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        runtimeLibs = with pkgs; [
          libGL
          libGLU
          vulkan-loader
          libdrm
          libx11
          libxext
          libxrandr
          libxi
          libxcursor
          libxfixes
          libxtst
          libxscrnsaver
          libxkbcommon
          wayland
          alsa-lib
          pipewire
          pulseaudio
          jack2
          sndio
          udev
          dbus
          fribidi
          libthai
        ];
      in
      {
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "Turret_2";
          version = "2.0.16";
          src = ./.;

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            pkg-config
            autoPatchelfHook
          ];

          buildInputs = runtimeLibs ++ [ pkgs.stdenv.cc.cc.lib pkgs.zlib ];

          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
          ];

          autoPatchelfIgnoreMissingDeps = [ "libsteam_api.so" "libGLES_CM.so.1" ];

          installPhase = ''
            runHook preInstall
            mkdir -p $out/bin
            cp Turret_2 $out/bin/
            cp -P ${./libs/GCC_so}/*.so* $out/bin/
            if [ -d ${./res} ]; then
              cp -r ${./res} $out/bin/res
            fi
            runHook postInstall
          '';

          dontStrip = false;
        };

        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            cmake
            ninja
            gcc
            gdb
            pkg-config
          ] ++ runtimeLibs;

          LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath runtimeLibs;

          shellHook = ''
            echo "Turret_2 dev shell: используйте 'cmake -B build -G Ninja && cmake --build build'"
          '';
        };
      });
}
