# -*- mode: python; -*-
# vim: set syntax=python:

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

def _vendored_git_repository_impl(repository_ctx):
  url = repository_ctx.attr.url
  schemaless_url = url.split("://", 1)[1]
  local_path = repository_ctx.workspace_root.get_child("vendor", schemaless_url)
  if local_path.get_child("WORKSPACE").exists or local_path.get_child("WORKSPACE.bazel").exists:
    # git submodule init --update was run.

    if repository_ctx.attr.build_file_content:
      native.new_local_repository(
        name = repository_ctx.attr.name,
        path = local_path,
        build_file_content = repository_ctx.attr.build_file_content,
      )
    else:
      native.local_repository(
        name = repository_ctx.attr.name,
        path = local_path,
      )
  else:
    # Vendored path is not a valid Bazel repo
    # Assume it's a submodule for now.
    # TODO: read .gitmodules to avoid the need for commit to be passed, and to detect an invalid submodule being defined.
    # TODO: if github, fetch with http_archive / http_file.

    if repository_ctx.attr.build_file_content:
      new_git_repository(
        name = repository_ctx.attr.name,
        url = repository_ctx.attr.url,
        commit = repository_ctx.attr.commit,
        tag = repository_ctx.attr.tag,
        build_file_content = repository_ctx.attr.build_file_content,
      )
    else:
      git_repository(
        name = repository_ctx.attr.name,
        url = repository_ctx.attr.url,
        commit = repository_ctx.attr.commit,
        tag = repository_ctx.attr.tag,
      )


_vendored_git_repository_rule = repository_rule(
  implementation = _vendored_git_repository_impl,
  attrs = {
    "url": attr.string(),
    # "sha256": attr.string(), # Only applicable if we use http_archive.
    "commit": attr.string(),
    "tag": attr.string(),

    "build_file_content": attr.string(), # If empty, assume not new_*_repository.
  },
)


def vendored_git_repository(name, url, commit = None, tag = None):
  _vendored_git_repository_rule(name = name, url = url, commit = commit, tag = tag)


def new_vendored_git_repository(name, url, build_file_content, commit = None, tag = None):
  _vendored_git_repository_rule(name = name, url = url, commit = commit, tag = tag, build_file_content = build_file_content)
