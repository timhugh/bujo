# Bujo

Bujo is a CLI for interacting with a markdown bullet journal.
At it's core, Bujo is just a file finder for your journal, which is intended to be used with your markdown editor of choice.

See the [setup examples](#setup-examples) for specifics on different editors, or come up with your own way to use it!

## Installation

```sh
gem install bujo
```

Bujo is written in Ruby 3.4. It hasn't been tested with older versions, but it should work with anything 3+

Dependencies vary by configuration:

- when using `search_adapter = fzf`, [junegunn/fzf](https://github.com/junegunn/fzf) must be installed and available in your path

## Usage

These examples will use the default configuration. See [configuration](#configuration) to learn how you can customize your journal.

- `bujo search` - opens a fuzzy finder using fzf and returns the selected file to STDOUT

## Configuration

Bujo config lives in the Toml file `~/.bujorc`. Please refer to the [in-source documentation](lib/bujo/config.rb) to learn about configuration options.

## Setup Examples

As noted above, the CLI itself does not do much on its own, but is designed to be paired with an editor for a full journal experience. These examples are not prescriptive and are only intended to give you some ideas for how you can leverage it on your own. Feel free to experiment!

<details>
<summary>Zed<summary>

In `~/.config/zed/tasks.json`:

```json
[
  {
    "label": "Bujo: search",
    "command": "zed $(bujo s)",
    "use_new_terminal": true,
    "allow_concurrent_runs": false,
    "reveal": "always",
    "hide": "on_success"
  },
  {
    "label": "Bujo: now",
    "command": "zed $(bujo now)",
    "use_new_terminal": true,
    "allow_concurrent_runs": false,
    "reveal": "no_focus",
    "hide": "on_success"
  },
  {
    "label": "Bujo: next",
    "command": "zed $(bujo next '$ZED_FILE')",
    "use_new_terminal": true,
    "allow_concurrent_runs": false,
    "reveal": "no_focus",
    "hide": "on_success"
  },
  {
    "label": "Bujo: previous",
    "command": "zed $(bujo previous '$ZED_FILE')",
    "use_new_terminal": true,
    "allow_concurrent_runs": false,
    "reveal": "no_focus",
    "hide": "on_success"
  }
]
```

The key here is that non-interactive tasks like `bujo now` don't reveal at all, and interactive tasks like `bujo search` will auto-close which makes them feel like a picker modal.

Then in `~/.config/zed/keymap.json` (note that I use `,` as a leader key, adjust to your tastes):

```json
[
  {
    "context": "Workspace",
    "bindings": {
      ", n n": ["task:Spawn", { "task_name": "Bujo: now" }],
      ", n f": ["task:Spawn", { "task_name": "Bujo: next" }],
      ", n b": ["task:Spawn", { "task_name": "Bujo: previous" }],
      ", n s": ["task:Spawn", { "task_name": "Bujo: search" }]
    }
  }
]
```

</details>

## History

I created [Bujo.nvim](https://github.com/timhugh/bujo.nvim) as sort of mishmash of Obsidian and Orgmode accessible from any project I have open in Neovim. I love using Neovim, but I also sometimes use other editors and I didn't love the friction of having to open up Neovim in a terminal to use my journal anytime I was moonlighting in Cursor or Zed.

The original idea for this was to just duplicate Bujo.nvim in Rust as a Zed extension, but Zed's extension API doesn't yet support custom commands and then I would have to duplicate it again to support VSCode, Cursor, Jetbrains, etc. so I needed a better approach. Luckily, 90% of the functionality of the Bujo.nvim plugin is actually just finding and opening files, which is a very trivial thing to do!

## License

The gem is available as open source under the terms of the [MIT License](https://opensource.org/licenses/MIT).
