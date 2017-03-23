
define internal void @run() {
  ret void
}

define void @main() {
  call void @run()
  ret void
}
