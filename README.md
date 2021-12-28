# X11 GUI

Graphical User Interface build with X11 (Xlib).

## Widgets

- TextInput
- Label
- ListView
- VLayout / HLayout
- Button

## Sample Application

```cpp
int main() {
  Application *app = new Application("Howdy!");

  Label la("Label");

  Button btn;
  btn.setLabel("Close");

  btn.setOnClick([app] {
    app->triggerExit();
  });

  VLayout layout;
  layout.addWidget(&la);
  layout.addWidget(&btn);

  layout.setFullSize();
  layout.setSpaceBetween(10);
  app->setLayout(layout);

  app->exec();
  return 0;
}
```

## Running

```console
$ make run
```
