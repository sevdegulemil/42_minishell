# minishell

**minishell**, C dilinde yazılmış, bash kabuğunu temel komutlarıyla taklit eden bir shell uygulaması. Bir Unix benzeri ortamda temel komutları çalıştırarak kullanıcı etkileşimi sağlar.

## İçerik

- [Özellikler](#özellikler)
- [Kurulum](#kurulum)
- [Kullanım](#kullanım)

## Özellikler

- Kullanıcıdan komut alabilme.
- Temel Unix komutlarının execute edilmesi.
- Built-in yönetimi.
- Birden fazla komutun ardışık olarak yürütülmesi.
- Basit hata yönetimi ve geri bildirim mekanizması.
- readline.h kullanımı
- linked list ile yönetim

## Kurulum

Bu projeyi kendi bilgisayarınızda çalıştırmak için aşağıdaki adımları izleyin:

1. Projeyi klonlayın:
   ```bash
   git clone https://github.com/kullanıcı_adı/minishell.git
2. Derleyin
   ```bash
   make

## Kullanım

1. minishell'i çalıştırmak için terminalde aşağıdaki komutu kullanın:
```bash
./minishell
("minishell$ " promptu görüntülenecektir. Sonrasında bash gibi kullanmaya devam edebilirsiniz.)

2. "echo, cd, pwd, export, unset, env, exit" builtinleri ele alınmıştır.
   "ctrl-C, ctrl-D and ctrl-\" sinyalleri handle edilmiştir.
   Redirectionlar (<, >, <<, >>) implement edilmiştir.
   $, $?, ", ' ele alındı.
   pipe (|) ele alınmıştır.
   working history mevcut.

Bunlara göre like bash olarak kullanılabilir.
