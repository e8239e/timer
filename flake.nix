{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    flake-compat.url = "github:edolstra/flake-compat";
    flake-compat.flake = false;
  };

  outputs = inputs@{ self, flake-utils, nixpkgs, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };
        Stdenv = pkgs.gcc13Stdenv;
      in
      {
        devShells.default = (pkgs.mkShell.override { stdenv = Stdenv; }) {
          nativeBuildInputs = with pkgs; [
            valgrind # test for memory errors & leaks
            bear # extract compile commands for lsp
            clang-tools # lsp (clangd)
            ccls # lsp
          ];
          buildInputs = with pkgs; [
            criterion # test parse_duration
            inih # config parsing
            sfml # play sound
          ];
        };
      });
}

