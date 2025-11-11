project(
  'ftn2xml', 'cpp',
  license: 'GPL-3.0-or-later',
  version: files('version.txt'),
  meson_version: '>=0.61',
  default_options: ['buildtype=release', 'prefix=/usr']
)

opt_cli = get_option('cli')
opt_install_lib = get_option('install_lib')
opt_install_docs = get_option('install_docs')

if meson.is_subproject()
  message('Building as subproject — disabling CLI, library, and docs')
  opt_cli = false
  opt_install_lib = false
  opt_install_docs = false
elif not opt_cli and not opt_install_lib
  error('Both cli=false and install_lib=false — nothing to build or install.')
endif

cli11_dep = dependency('CLI11', required: opt_cli)

podofo_dep = dependency('libpodofo', required: get_option('podofo'))
if podofo_dep.found()
  core_sources = [ 'source/renderers_pdf.cc' ]
  add_project_arguments('-DHAVE_PODOFO', language: 'cpp')
endif

conf_data = configuration_data()
conf_data.set('version', meson.project_version())
conf_data.set('project_datadir', get_option('datadir') / meson.project_name())

config_h = configure_file(
  input: 'config.h.in',
  output: 'config.h',
  configuration: conf_data
)

# Core sources (modular layout)
core_sources += [
  config_h,
  'source/utils_file.cc',
  'source/utils_string.cc',
  'source/model_script.cc',
  'source/parser_fountain.cc',
  'source/renderers_html.cc',
  'source/renderers_fdx.cc',
  'source/renderers_screenplain.cc',
  'source/renderers_textplay.cc',
  'source/renderers_xml.cc',
]

# Static library
fountain_lib = static_library(
  meson.project_name(),
  core_sources,
  dependencies: [podofo_dep],
  install: opt_install_lib
)

# Export dependency object for subproject use
ftn2xml_dep = declare_dependency(
  link_with: fountain_lib,
  include_directories: include_directories('source'),
  dependencies: [podofo_dep]
)

# Install headers + pkg-config if requested
if opt_install_lib
  install_headers(
    'source/utils_file.h',
    'source/utils_string.h',
    'source/model_script.h',
    'source/parser_fountain.h',
    'source/renderers_html.h',
    'source/renderers_fdx.h',
    'source/renderers_screenplain.h',
    'source/renderers_textplay.h',
    'source/renderers_xml.h',
    get_option('pdf_export').enabled() ? 'source/renderers_pdf.h' : [],
    subdir: meson.project_name()
  )

  pkg_mod = import('pkgconfig')
  pkg_mod.generate(
    name: meson.project_name(),
    description: 'Fountain screenplay format parser',
    version: meson.project_version(),
    libraries: fountain_lib,
    subdirs: meson.project_name(),
    requires_private: podofo_dep.found() ? ['libpodofo'] : []
  )
endif

# CLI executable
if opt_cli
  exe = executable(
    meson.project_name(),
    sources: ['source/main.cc'],
    dependencies: [cli11_dep, podofo_dep],
    link_with: fountain_lib,
    install: true
  )

  if meson.version().version_compare('>=0.61.0')
    foreach alias : ['ftn2fdx', 'ftn2html']
      install_symlink(alias, install_dir: get_option('bindir'), pointing_to: meson.project_name())
    endforeach
    if get_option('pdf_export').enabled()
      install_symlink('ftn2pdf', install_dir: get_option('bindir'), pointing_to: meson.project_name())
    endif
  endif

  # CSS files only for CLI
  install_data(
    ['data/fountain-html.css', 'data/fountain-xml.css', 'data/screenplain.css', 'data/textplay.css'],
    install_dir: get_option('datadir') / meson.project_name()
  )
endif

# Docs behind option
if opt_install_docs
  install_data(
    ['Fountain_Syntax.md', 'Readme.md'],
    install_dir: get_option('datadir') / 'doc' / meson.project_name()
  )
endif
