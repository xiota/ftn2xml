project(
  'ftn2xml', 'cpp',
  license: 'GPL-3.0-or-later',
  version: files('version.txt'),
  meson_version: '>=0.61',
  default_options: ['buildtype=release', 'prefix=/usr']
)

# Safety net
if not get_option('cli') and not get_option('install_lib')
  error('Both cli=false and install_lib=false — nothing to build or install.')
endif

podofo = dependency('libpodofo', required: get_option('pdf_export'))
cli11_dep = dependency('CLI11', required: get_option('cli'))

conf_data = configuration_data()
conf_data.set('version', meson.project_version())
conf_data.set('project_datadir', get_option('datadir') / meson.project_name())
conf_data.set10('pdf_export', podofo.found())

config_h = configure_file(
  input: 'config.h.in',
  output: 'config.h',
  configuration: conf_data
)

# Core sources (modular layout)
core_sources = [
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

if get_option('pdf_export').enabled()
  core_sources += 'source/renderers_pdf.cc'
endif

# Static library
fountain_lib = static_library(
  meson.project_name(),
  core_sources,
  dependencies: [podofo],
  install: get_option('install_lib')
)

# Install headers + pkg-config if requested
if get_option('install_lib')
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
    requires_private: podofo.found() ? ['libpodofo'] : []
  )
endif

# CLI executable
if get_option('cli')
  exe = executable(
    meson.project_name(),
    sources: ['source/main.cc'],
    dependencies: [cli11_dep, podofo],
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
if get_option('install_docs')
  install_data(
    ['Fountain_Syntax.md', 'Readme.md'],
    install_dir: get_option('datadir') / 'doc' / meson.project_name()
  )
endif
