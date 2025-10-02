#!/usr/bin/env python3
"""
GitHub Issues Bulk Creator

Parses issues.yaml and creates GitHub issues with milestones and labels.
Uses GitHub CLI (gh) for authentication.

Usage:
    python3 create_issues.py [--dry-run]

Requirements:
    - PyYAML: pip install pyyaml
    - GitHub CLI (gh) installed and authenticated
"""

import argparse
import json
import subprocess
import sys
from typing import Dict, List, Optional

try:
    import yaml
except ImportError:
    print("Error: PyYAML not installed. Run: pip install pyyaml")
    sys.exit(1)


class GitHubIssueCreator:
    def __init__(self, repo: str, dry_run: bool = False):
        self.repo = repo
        self.dry_run = dry_run
        self.milestone_cache: Dict[str, int] = {}
        self.label_cache: List[str] = []

    def run_gh_command(self, args: List[str]) -> Optional[str]:
        """Execute GitHub CLI command and return output."""
        try:
            result = subprocess.run(
                ["gh"] + args,
                capture_output=True,
                text=True,
                check=True
            )
            return result.stdout.strip()
        except subprocess.CalledProcessError as e:
            print(f"Error running gh command: {e.stderr}")
            return None

    def create_or_get_milestone(self, title: str) -> Optional[int]:
        """Create milestone if it doesn't exist, return milestone number."""
        # Check cache first
        if title in self.milestone_cache:
            return self.milestone_cache[title]

        if self.dry_run:
            print(f"  [DRY RUN] Would create milestone: {title}")
            self.milestone_cache[title] = 1  # Fake ID for dry run
            return 1

        # Check if milestone exists
        output = self.run_gh_command([
            "api",
            f"repos/{self.repo}/milestones",
            "--jq",
            f'.[] | select(.title == "{title}") | .number'
        ])

        if output and output.strip():
            milestone_number = int(output.strip())
            print(f"  ✓ Milestone exists: {title} (#{milestone_number})")
            self.milestone_cache[title] = milestone_number
            return milestone_number

        # Create milestone
        print(f"  ↻ Creating milestone: {title}")
        output = self.run_gh_command([
            "api",
            f"repos/{self.repo}/milestones",
            "-f", f"title={title}",
            "--jq", ".number"
        ])

        if output:
            milestone_number = int(output.strip())
            print(f"  ✓ Created milestone: {title} (#{milestone_number})")
            self.milestone_cache[title] = milestone_number
            return milestone_number

        return None

    def create_label_if_missing(self, label: str, color: str = "0366d6") -> bool:
        """Create label if it doesn't exist."""
        # Check cache first
        if label in self.label_cache:
            return True

        if self.dry_run:
            print(f"  [DRY RUN] Would create label: {label}")
            self.label_cache.append(label)
            return True

        # Check if label exists
        output = self.run_gh_command([
            "label",
            "list",
            "--json", "name",
            "--jq", f'.[] | select(.name == "{label}") | .name'
        ])

        if output and output.strip() == label:
            self.label_cache.append(label)
            return True

        # Create label
        print(f"  ↻ Creating label: {label}")
        result = self.run_gh_command([
            "label",
            "create",
            label,
            "--color", color
        ])

        if result is not None:
            print(f"  ✓ Created label: {label}")
            self.label_cache.append(label)
            return True

        return False

    def create_issue(self, issue_data: Dict) -> bool:
        """Create a GitHub issue."""
        title = issue_data.get("title")
        body = issue_data.get("body", "")
        labels = issue_data.get("labels", [])
        milestone = issue_data.get("milestone")

        if not title:
            print("  ✗ Skipping issue without title")
            return False

        print(f"\n→ Processing: {title}")

        # Get milestone number if specified
        milestone_number = None
        if milestone:
            milestone_number = self.create_or_get_milestone(milestone)
            if not milestone_number:
                print(f"  ✗ Failed to create/get milestone: {milestone}")
                return False

        # Ensure labels exist
        for label in labels:
            if not self.create_label_if_missing(label):
                print(f"  ⚠ Warning: Could not create label: {label}")

        if self.dry_run:
            print(f"  [DRY RUN] Would create issue:")
            print(f"    Title: {title}")
            print(f"    Labels: {', '.join(labels)}")
            print(f"    Milestone: {milestone}")
            return True

        # Build gh issue create command
        cmd = ["issue", "create", "--title", title, "--body", body]

        if labels:
            cmd.extend(["--label", ",".join(labels)])

        if milestone:
            cmd.extend(["--milestone", milestone])

        # Create issue
        print(f"  ↻ Creating issue...")
        output = self.run_gh_command(cmd)

        if output:
            print(f"  ✓ Created: {output}")
            return True
        else:
            print(f"  ✗ Failed to create issue")
            return False

    def process_yaml(self, yaml_file: str):
        """Parse YAML and create all issues."""
        print(f"\n{'='*60}")
        print(f"GitHub Issue Bulk Creator")
        print(f"{'='*60}")
        print(f"Repository: {self.repo}")
        print(f"Mode: {'DRY RUN' if self.dry_run else 'LIVE'}")
        print(f"{'='*60}\n")

        # Load YAML
        try:
            with open(yaml_file, 'r') as f:
                data = yaml.safe_load(f)
        except Exception as e:
            print(f"Error loading YAML: {e}")
            sys.exit(1)

        issues = data.get("issues", [])

        if not issues:
            print("No issues found in YAML file")
            return

        print(f"Found {len(issues)} issues to create\n")

        # Track statistics
        created = 0
        failed = 0

        for issue in issues:
            if self.create_issue(issue):
                created += 1
            else:
                failed += 1

        # Summary
        print(f"\n{'='*60}")
        print(f"Summary")
        print(f"{'='*60}")
        print(f"Total issues: {len(issues)}")
        print(f"✓ Created: {created}")
        print(f"✗ Failed: {failed}")

        if self.dry_run:
            print(f"\nThis was a DRY RUN. No issues were actually created.")
            print(f"Run without --dry-run to create issues.")

        print(f"{'='*60}\n")


def main():
    parser = argparse.ArgumentParser(
        description="Bulk create GitHub issues from YAML file"
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Preview what would be created without making changes"
    )
    parser.add_argument(
        "--repo",
        default="Zabooya/HandEvaluation",
        help="GitHub repository (owner/repo format)"
    )
    parser.add_argument(
        "--file",
        default="issues.yaml",
        help="Path to YAML file (default: issues.yaml)"
    )

    args = parser.parse_args()

    # Verify gh CLI is installed
    try:
        subprocess.run(
            ["gh", "--version"],
            capture_output=True,
            check=True
        )
    except (subprocess.CalledProcessError, FileNotFoundError):
        print("Error: GitHub CLI (gh) not found or not authenticated")
        print("Install: https://cli.github.com/")
        print("Authenticate: gh auth login")
        sys.exit(1)

    # Create issues
    creator = GitHubIssueCreator(args.repo, args.dry_run)
    creator.process_yaml(args.file)


if __name__ == "__main__":
    main()
