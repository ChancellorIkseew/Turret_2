## Nix

Добавить в свой системный `flake.nix`:

```nix
{
  inputs = {
    turret_2.url = "github:ChancellorIkseew/Turret_2";
  };

  outputs = { self, nixpkgs, turret_2, ... }: {
    nixosConfigurations.<hostname> = nixpkgs.lib.nixosSystem {
      # ...
      modules = [
        {
          environment.systemPackages = [
            turret_2.packages.${pkgs.system}.default
          ];
        }
      ];
    };
  };
}
```

Или локально:

```sh
nix build
./result/bin/Turret_2
```

Для разработки:

```sh
nix develop
cmake -B build -G Ninja && cmake --build build
```
